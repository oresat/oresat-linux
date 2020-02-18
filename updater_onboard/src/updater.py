#!/usr/bin/env python3


from pathlib import Path
from enum import Enum
import threading, os, sys, re, yaml, subprocess, apt_pkg, apt.debfile, shutil, time, syslog, tarfile
from updater_state_machine import UpdaterStateMachine, State


CACHE_DIR = '/tmp/oresat-linux-updater/cache/'
WORKING_DIR = '/tmp/oresat-linux-updater/working/'


class LinuxUpdater(object):
    def __init__(self):
        # make directories for updater, if not found
        Path(CACHE_DIR).mkdir(parents=True, exist_ok=True)
        Path(WORKING_DIR).mkdir(parents=True, exist_ok=True)

        # state machine set up
        self._state_machine = UpdaterStateMachine()
        self._state_machine.current_state = State.SLEEP.value

        # archive fields set up
        self._archive_file_name = ""
        self._available_archive_files = len(os.listdir(CACHE_DIR))

        # thread set up
        self.__lock = threading.Lock()
        self.__running = True
        self.__working_thread = threading.Thread(target=self.__working_loop)
        self.__working_thread.start() # start working thread


    def __del__(self):
        """ del updater process """
        self.quit()


    def quit(self):
        """ Use to stop all threads nicely """
        self.__running = False
        if self.__working_thread.is_alive():
            self.__working_thread.join()


    # ------------------------------------------------------------------------
    # dbus properties


    @property
    def current_state(self):
        return self._state_machine.current_state


    @property
    def current_archive_file(self):
        return self._archive_file_name


    @property
    def available_archive_files(self):
        return self._available_archive_files


    # ------------------------------------------------------------------------
    # dbus methods


    def add_archiveFile(self, file_path):
        """ copies file into CACHE_DIR """
        if(file_path[0] != '/'):
            syslog.syslog(syslog.LOG_ERR, "not an absolute path: " + file_path)
            return False

        # TODO fix this
        """
        # check for valid update file
        if not re.match(r"(update-\d\d\d\d-\d\d-\d\d-\d\d-\d\d\.tar\.gz)", self.__archive_file_path):
            syslog.syslog(syslog.LOG_ERR, "Not a valid update file")
            return
        """

        self.__lock.acquire()
        ret = shutil.copy(file_path, CACHE_DIR)

        if CACHE_DIR in ret:
            self._available_archive_files = len(os.listdir(CACHE_DIR)) # not += 1
            # this will handle file overrides
            self.__lock.release()
            return True

        self.__lock.release()
        return False # failed to copy


    def start_update(self):
        """ Start updaing if in sleep state """
        rv = True

        if self._state_machine.current_state == State.SLEEP.value:
            self.__lock.acquire()
            self._state_machine.change_state(State.UPDATE.value)
            self.__lock.release()
        else:
            rv = False

        return True


    def get_apt_list_output(self):#TODO
        """ To stop updaing """
        return True


    # ------------------------------------------------------------------------
    # non-dbus class methods


    def __working_loop(self):
        while(self.__running):
            if self._state_machine.current_state == State.SLEEP.value:
                self.__failed_state()
            elif self._state_machine.current_state == State.UPDATE.value:
                self.__update_state()
            else:
                time.sleep(1)


    def __failed_state(self, err):
        """
        Update failed reverting all part of update and clearing
        working and update directories.
        """

        self.__lock.acquire()

        # remove all updater files
        shutil.rmtree(WORKING_DIR)
        shutil.rmtree(CACHE_DIR)
        Path(WORKING_DIR).mkdir(parents=True, exist_ok=True)
        Path(CACHE_DIR).mkdir(parents=True, exist_ok=True)

        self._available_archive_files = 0
        self._archive_file_name = ""

        self.__lock.release()

        # TODO revert update here

        self.__lock.acquire()
        self._state_machine.change_state(State.SLEEP.value)
        self.__lock.release()


    def __update_state(self):
        """
        Load oldest update file if one exist
        Update thread function.
        Finds the oldest update file and starts update
        """

        archive_instruction = {}
        archive_file_path = ""

        # see if any update file exist
        list_of_files = os.listdir(CACHE_DIR)
        if not list_of_files:
            self.__lock.acquire()
            self._state_machine.change_state(State.SLEEP.value)
            self.__lock.release()
            return # done, empty, no update files

        self.__lock.acquire()
        self._archive_file_name = list_of_files[0] # get 1st file
        archive_file_path = CACHE_DIR + self._archive_file_name
        self.__lock.release()

        # copy file into working dir
        ret_path = shutil.copy(archive_file_path, WORKING_DIR)
        if CACHE_DIR in ret_path:
            syslog.syslog(syslog.LOG_ERR, "Failed to copy into working dir")
            return

        # open_archive_file update file
        t = tarfile.open(ret_path, "r:gz")
        if not t:
            syslog.syslog(syslog.LOG_ERR, "Open archive failed")
            return
        else:
            t.extractall()
            t.close()

        # load instructins file
        try:
            with open(WORKING_DIR + "instructions.yml", "r") as file_object:
                # load instructions data into dictionary
                archive_instruction = yaml.safe_load(file_object)
                if not archive_instruction:
                    syslog.syslog(syslog.LOG_ERR, "Not a valid yml file")
                    return
        except IOError:
            syslog.syslog(syslog.LOG_ERR, "Instructions file not accessible")
            return

        # remove packages
        if "remove-package" in archive_instruction:
            pkgs = archive_instruction["remove-packages"]
            for p in pkgs:
                if not remove_pkg(WORKING_DIR + p):
                    syslog.syslog(syslog.LOG_ERR, "Package " + p + " failed to be removed.")

        # install packages
        if "install-package" in archive_instruction:
            pkgs = archive_instruction["install-packages"]
            for p in pkgs:
                if not install_pkg(WORKING_DIR + p):
                    syslog.syslog(syslog.LOG_ERR, "Package " + p + " failed to be installed.")

        self.__lock.acquire()

        # clear working dir and remove update deb pkg
        shutil.rmtree(WORKING_DIR)
        Path(WORKING_DIR).mkdir(parents=True, exist_ok=True)
        os.remove(archive_file_path)
        self._archive_file_name = ""
        self._available_archive_files -= 1
        self._state_machine.change_state(State.SLEEP.value)

        self.__lock.release()
        return


def install_pkg(file_path):
    """ output will be 0 if it completes install, anything else fails """
    deb = apt.debfile.DebPackage(file_path)
    if deb.check() and deb.install() == 0: # valid package and install worked
        return True
    return False


def remove_pkg(file_path): # TODO change to python3-apt
    bashCommand = "sudo apt-get -qq remove ./"+ file_path
    output = subprocess.check_call(['bash','-c', bashCommand])
    return True

LinuxUpdater()
