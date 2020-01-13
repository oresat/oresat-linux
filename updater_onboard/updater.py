#!/usr/bin/env python3


from pathlib import Path
from pydbus.generic import signal
from pydbus import SystemBus
from gi.repository import GLib
from enum import Enum
import threading, os, sys, re, yaml, subprocess, apt_pkg, apt.debfile, shutil, time


DBUS_INTERFACE_NAME = "org.OreSat.Updater"
UPDATES_DIR = '/tmp/oresat-linux-updater/archives/'
WORKING_DIR = '/tmp/oresat-linux-updater/working/'


# all state for linux updater, uses auto() since we don't care about the actual value
class State(Enum):
    FAILED = 0
    SLEEP = 1
    UPDATE = 2


class LinuxUpdater(object):
    dbus = """
    <node>
        <interface name="org.OreSat.Updater">
            <property name="CurrentState" type="d" access="read"/>
            <property name="ComputerName" type="s" access="readwrite"/>
            <property name="CurrentArchiveFile" type="s" access="read"/>
            <property name="AvailableArchiveFiles" type="d" access="read"/>
            <property name="ErrorMessage" type="s" access="read"/>
            <method name='AddArchiveFile'>
                <arg type='s' name='file_path' direction='in'/>
                <arg type='b' name='output' direction='out'/>
            </method>
            <method name='StartUpdate'>
                <arg type='b' name='output' direction='out'/>
            </method>
            <method name='GetAptListOutput'>
                <arg type='b' name='output' direction='out'/>
            </method>
        </interface>
    </node>
    """

    def __init__(self):
        # make directories for updater, if not found
        Path(UPDATES_DIR).mkdir(parents=True, exist_ok=True) 
        Path(WORKING_DIR).mkdir(parents=True, exist_ok=True)

        # set computer_name to hostname
        self._computer_name = "" # encase hostname is missing
        with open("/etc/hostname", "r") as f:
            self._computer_name = f.readline().strip()

        # set local fields
        self._current_state = State.SLEEP.value
        self._error_message = ""
        self._archive_file_name = ""
        self._available_archive_files = len(os.listdir(UPDATES_DIR))
        self.__lock = threading.Lock()
        self.__running = True
        self.__working_thread = threading.Thread(target=self.__working_loop)

        # start working thread
        self.__working_thread.start()


    def __del__(self):
        """ del updater process """
        self.end()


    # ------------------------------------------------------------------------
    # dbus properties


    @property
    def CurrentState(self):
        return self._current_state


    @property
    def ErrorMessage(self):
        return self._error_message


    @property
    def ComputerName(self):
        return self._computer_name


    @ComputerName.setter
    def ComputerName(self, value):
        self.__lock.acquire()
        self._computer_name = value.lower()
        self.__lock.release()


    @property
    def CurrentArchiveFile(self):
        return self._archive_file_name


    @property
    def AvailableArchiveFiles(self):
        return self._available_archive_files


    # ------------------------------------------------------------------------
    # dbus methods


    def AddUpdateFile(self, file_path):
        """ copies file into UPDATES_DIR """
        if(file_path[0] != '/'):
            self.__update_error("not an absolute path: " + file_path)
            return False

        # TODO fix this
        """
        # check for valid update file
        if not re.match(r"(update-\d\d\d\d-\d\d-\d\d-\d\d-\d\d\.tar\.gz)", self.__archive_file_path):
            self.__update_error("Not a valid update file")
            return
        """

        self.__lock.acquire()
        ret = shutil.copy(file_path, UPDATES_DIR)

        if UPDATES_DIR in ret:
            self._available_archive_files = len(os.listdir(UPDATES_DIR)) # not += 1
            # this will handle file overrides
            self.__lock.release()
            return True

        self.__lock.release()
        return False # failed to copy


    def StartUpdate(self):
        """ Start updaing if in sleep state """
        rv = True

        if(self._current_state == State.SLEEP.value):
            self.__lock.acquire()
            self._current_state = State.UPDATE.value)
            self.__lock.release()
        else:
            rv = False

        return True


    def GetAptListOutput(self):#TODO
        """ To stop updaing """
        return True


    # ------------------------------------------------------------------------
    # non-dbus class methods


    def __working_loop(self):
        while(self.__running):
            if self._current_state == State.FAILED.value:
                self.__failed_state()
            elif self._current_state == State.UPDATE.value:
                self.update()
            else:
                time.sleep(1)


    def __update_error(self, message):
        self.__lock.acquire()
        self._error_message = message
        self._current_state = State.ERROR.value
        self.__lock.release()


    def end(self):
        """ Use to stop all threads nicely """
        self.__running = False
        self._current_state(State.STOP.value) # stop if currently updating
        if self.__working_thread.is_alive():
            self.__working_thread.join()


    def __failed_state(self, err):
        """ 
        Update failed reverting all part of update and clearing 
        working and update directories.
        """

        self.__lock.acquire()

        # remove all updater files 
        shutil.rmtree(WORKING_DIR)
        shutil.rmtree(UPDATES_DIR)
        Path(WORKING_DIR).mkdir(parents=True, exist_ok=True)
        Path(UPDATES_DIR).mkdir(parents=True, exist_ok=True) 
        
        #reset fields not reset by __update_error()
        self._available_archive_files = 0
        self._archive_file_name = ""

        self.__lock.release()

        # TODO revert update here

        self.__lock.acquire()
        self._current_state = State.SLEEP.value
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
        list_of_files = os.listdir(UPDATES_DIR)
        if not list_of_files:
            self.__lock.acquire()
            self._current_state = State.SLEEP.value
            self.__lock.release()
            return # done, empty, no update files

        self.__lock.acquire()
        self._archive_file_name = list_of_files[0] # get 1st file
        archive_file_path = UPDATES_DIR + self._archive_file_name
        self.__lock.release()

        # copy file into working dir
        ret_path = shutil.copy(archive_file_path, WORKING_DIR)
        if UPDATES_DIR in ret_path:
            self.__update_error("Failed to copy into working dir")
            return

        # open_archive_file update file
        if not open_archive_file(ret_path):
            self.__update_error("Open archive failed")
            return

        # load instructins file
        try:
            with open(WORKING_DIR + "instructions.yml", "r") as file_object:
                # load instructions data into dictionary
                archive_instruction = yaml.safe_load(file_object)
                if not archive_instruction:
                    self.__update_error("Not a valid yml file")
                    return
        except IOError:
            self.__update_error("Instructions file not accessible")
            return

        # remove packages
        self.__parse_remove_instructions("all", archive_instruction)
        self.__parse_remove_instructions(self.computer_name, archive_instruction)

        # install packages
        self.__parse_install_instructions("all", archive_instruction)
        self.__parse_install_instructions(self.computer_name, archive_instruction)

        self.__lock.acquire()

        # clear working dir and remove update deb pkg
        shutil.rmtree(WORKING_DIR)
        Path(WORKING_DIR).mkdir(parents=True, exist_ok=True)
        os.remove_pkg(archive_file_path)
        self._archive_file_name = ""
        self._available_archive_files -= 1
        self._current_state = State.SLEEP.value
        
        self.__lock.release()
        return


    def __parse_install_instructions(self, computer_name, archive_instruction):
        """ follows the insstruction for computr name in the archive instruction """
        install_string = name + "-install-pkgs"
        if install_string not in archive_instruction: 
            return False # no instruction for that computer name

        pkgs = archive_instruction[install_string]
        for p in pkgs:
            if not instal_pkg(WORKING_DIR + p):
                self.__update_error("install pkg failed: " + p)
                break False

        return True


    def __parse_remove_instructions(self, computer_name, archive_instruction):
        """ follows the insstruction for computr name in the archive instruction """
        remove_string = name + "-remove-pkgs"
        if remove_string not in archive_instruction: 
            return False # no instruction for that computer name

        pkgs = archive_instruction[remove_string]
        for p in pkgs:
            if not remove_pkg(WORKING_DIR + p):
                self.__update_error("remove pkg failed: " + p)
                break False

        return True


def instal_pkg(file_path):
    """ output will be 0 if it completes install, anything else fails """
    deb = apt.debfile.DebPackage(file_path)
    if deb.check() and deb.install() == 0: # valid package and install worked
        return True
    return False


def remove_pkg(file_path): # TODO change to python3-apt
    bashCommand = "sudo apt-get -qq remove ./"+ file_path
    output = subprocess.check_call(['bash','-c', bashCommand])
    return True


def open_archive_file(file_path): # TODO FLIF?
    """
    contents of .tar.gz file are extracted to a directory with the same
    name as the tar file file without the .tar.gz extension
    """
    
    bashCommand = "tar -xzf " + file_path + " -C " + WORKING_DIR
    output = subprocess.check_call(['bash','-c', bashCommand])

    #create a str with the directory name by slicing off the .tar.gz extension
    return True # file_name[0:len(file_path)-7]


def start_linux_updater():
    bus = SystemBus()
    loop = GLib.MainLoop()
    emit = LinuxUpdater()
    bus.publish(DBUS_INTERFACE_NAME, emit)

    try:
        loop.run()
    except KeyboardInterrupt as e:
        loop.quit()
        emit.end()
        print("\nExit by Control C")

