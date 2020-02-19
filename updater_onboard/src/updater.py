#!/usr/bin/env python3


from pathlib import Path
from enum import Enum
import threading, os, sys, re, subprocess, shutil, time, syslog, tarfile
from updater_state_machine import UpdaterStateMachine, State
from updater_apt import UpdaterApt


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

        # apt set up
        self._updater_apt = UpdaterApt()

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
        self.__running = False
        if self.__working_thread.is_alive():
            self.__working_thread.join()


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
        # (str) -> bool
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
        # () -> bool
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
        # () -> bool
        return True


    # ------------------------------------------------------------------------
    # non-dbus class methods


    def __working_loop(self):
        # () -> bool
        while(self.__running):
            if self._state_machine.current_state == State.FAILED.value:
                time.sleep(1)
            elif self._state_machine.current_state == State.SLEEP.value:
                time.sleep(1)
            elif self._state_machine.current_state == State.PREUPDATE.value:
                self.__pre_update()
            elif self._state_machine.current_state == State.UPDATE.value:
                self.__update()
            elif self._state_machine.current_state == State.REVERT.value:
                self.__revert()
            elif self._state_machine.current_state == State.FORCE.value:
                self.__force()
            else: # should not happen
                syslog.syslog(syslog.LOG_ERR, "current_state is set to an unknowned state.")
                self.__lock.acquire()
                self._state_machine.change_state(State.FAILED.value)
                self.__lock.release()


    def __pre_update(self):
        # () -> bool
        """
        Load oldest update file if one exist
        Update thread function.
        Finds the oldest update file and starts update
        """

        archive_file_path = ""

        # see if any update file exist
        list_of_files = os.listdir(CACHE_DIR)
        if not list_of_files:
            self.__lock.acquire()
            self._state_machine.change_state(State.SLEEP.value)
            self.__lock.release()
            return True # done, empty, no update files

        self.__lock.acquire()
        self._archive_file_name = list_of_files[0] # get 1st file
        archive_file_path = CACHE_DIR + self._archive_file_name
        self.__lock.release()

        # copy file into working dir
        ret_path = shutil.copy(archive_file_path, WORKING_DIR)
        if CACHE_DIR in ret_path:
            syslog.syslog(syslog.LOG_ERR, "Failed to copy into working dir")
            return False

        # open_archive_file update file
        t = tarfile.open(ret_path, "r:gz")
        if not t:
            syslog.syslog(syslog.LOG_ERR, "Open archive failed")
            return False
        else:
            t.extractall()
            t.close()

        self.__lock.acquire()
        self._state_machine.change_state(State.UPDATE.value)
        self.__lock.release()
        return True


    def __update(self):
        # () -> bool
        """
        Load oldest update file if one exist
        Update thread function.
        Finds the oldest update file and starts update
        """

        if not self.__parse_update_file():
            self.__lock.acquire()
            self._state_machine.change_state(State.REVERT.value)
            self.__lock.release()
            return False

        self.__lock.acquire()

        # clear working dir and remove update deb pkg
        shutil.rmtree(WORKING_DIR)
        Path(WORKING_DIR).mkdir(parents=True, exist_ok=True)
        os.remove(archive_file_path)
        self._archive_file_name = ""
        self._available_archive_files -= 1
        self._state_machine.change_state(State.SLEEP.value)

        self.__lock.release()
        return True


    def __revert(self, err):
        # (str) -> bool
        """
        Update failed reverting all part of update and clearing
        working and update directories.
        """

        # TODO do revert

        self.__lock.acquire()

        # remove all updater files
        shutil.rmtree(WORKING_DIR)
        shutil.rmtree(CACHE_DIR)
        Path(WORKING_DIR).mkdir(parents=True, exist_ok=True)
        Path(CACHE_DIR).mkdir(parents=True, exist_ok=True)

        self._available_archive_files = 0
        self._archive_file_name = ""


        self._state_machine.change_state(State.SLEEP.value)
        self.__lock.release()
        return True


    def __force(self):
        # () -> bool
        """
        """

        if not self.__parse_update_file():
            self.__lock.acquire()
            self._state_machine.change_state(State.FAILED.value)
            self.__lock.release()
            return False

        self.__lock.acquire()

        # clear working dir and remove update deb pkg
        shutil.rmtree(WORKING_DIR)
        Path(WORKING_DIR).mkdir(parents=True, exist_ok=True)
        os.remove(archive_file_path)
        self._archive_file_name = ""
        self._available_archive_files -= 1
        self._state_machine.change_state(State.SLEEP.value)

        self.__lock.release()
        return True


    def __parse_update_file(self):
        # () -> bool
        """
        Install all deb files in WORKING_DIR, remove all packages listed in
        remove.txt, and run all bash script in WORKING_DIR.
        """

        deb_files = []
        remove_pkgs = []

        # install packages
        for file in glob.glob(WORKING_DIR + "*.deb"):
            deb_files.append(file)

        if deb_files.size > 0:
            if not self._updater_apt.remove_packages(deb_files):
                syslog.syslog(syslog.LOG_ERR, "Remove package failed.")
                return False

        # remove packages
        with open(WORKING_DIR + 'remove.txt', 'r') as f:
            pkg = f.readline().strip()
            if not self._updater_apt.install_package(pkg):
                syslog.syslog(syslog.LOG_ERR, "Install package failed.")
                return False

        # run bash scripts
        for file in glob.glob(WORKING_DIR + "*.sh"):
            if subprocess.run(["/bin/bash/ ", file]) != 0:
                syslog.syslog(syslog.LOG_ERR, file + " exited with failure.")
                return False

        return True


LinuxUpdater()
