#!/usr/bin/env python3
"""
The updater file contains functions for installing updates for OreSat from
a .tar file. The primary function can be called using the following format. 

The name of the updatefile.tar does not matter as long as it is a .tar.gz file.
The updatefile.tar.gz should contain one or more .deb files. This is the only
file type that will be installed by this function.

The function will have an associated, permament directory labled: old_updates. 
Once a .deb file has been installed, it will be moved to this directory to be 
saved. The .tar.gz file and the tar directory will also be moved to this directory
once the process has been completed. 
"""


from pathlib import Path
from pydbus.generic import signal
from pydbus import SystemBus
from gi.repository import GLib
from enum import Enum
import threading, os, sys, re, yaml, subprocess, apt_pkg, apt.debfile, shutil, time


DBUS_INTERFACE_NAME = "org.OreSat.Updater"
UPDATES_DIR = '/tmp/oresat-linux-updater/updates/'
WORKING_DIR = '/tmp/oresat-linux-updater/working/'


# all state for linux updater, uses auto() since we don't care about the actual value
class State(Enum):
    ERROR = 0
    SLEEP = 1 
    PREUPDATE = 2 
    UPDATE = 3
    STOP = 4


class LinuxUpdater(object):
    dbus = """
    <node>
        <interface name="org.OreSat.Updater">
            <signal name="Error">
                <arg type='s'/>
            </signal>
            <property name="Status" type="d" access="read"/>
            <property name="ComputerName" type="s" access="readwrite"/>
            <property name="CurrentUpdateFile" type="s" access="read"/>
            <property name="UpdatesAvailable" type="d" access="read"/>
            <property name="ErrorMessage" type="s" access="read"/>
            <method name='AddUpdateFile'>
                <arg type='s' name='file_path' direction='in'/>
                <arg type='b' name='output' direction='out'/>
            </method>
            <method name='StartUpdate'>
                <arg type='b' name='output' direction='out'/>
            </method>
            <method name='Reset'>
                <arg type='b' name='output' direction='out'/>
            </method>
            <method name='EmergencyStopUpdate'>
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

        # set local fields
        self.lock = threading.Lock()
        self.current_state = State.SLEEP.value
        self.computer_name = "StarTracker"
        self.error_message = ""
        self.available_updates = len(os.listdir(UPDATES_DIR))
        self.running = True
        self.update_file_name = ""
        self.update_file_path = ""
        self.update_instruction = {}
        self.thread1 = threading.Thread(target=self.working_thread)

        # start working thread
        self.thread1.start()


    def __del__(self):
        """ del updater process """
        self.end()


    # dbus signals
    Error = signal()


    # dbus properties
    @property
    def Status(self):
        return self.current_state


    @property
    def ErrorMessage(self):
        return self.error_message


    @property
    def ComputerName(self):
        return self.computer_name


    @ComputerName.setter
    def ComputerName(self, value):
        self.computer_name = value


    @property
    def CurrentUpdateFile(self):
        return self.update_file_name


    @property
    def UpdatesAvailable(self):
        return self.available_updates


    # dbus methods
    def AddUpdateFile(self, file_path):
        """ copies file into UPDATES_DIR """
        if(file_path[0] != '/'):
            self.error("not an absolute path: " + file_path)
            return False
        
        ret = shutil.copy(file_path, UPDATES_DIR)
        
        if UPDATES_DIR in ret:
            self.available_updates = len(os.listdir(UPDATES_DIR)) # not += 1
            # therefor it will handle file overrides
            # file overrides should not happen
            return True
        
        return False # failed to copy


    def StartUpdate(self):
        """ To start updaing """
        return self.change_state(State.PREUPDATE.value)


    def Reset(self):
        """ Reset updater if in error state back into sleeping state """
        return self.change_state(State.SLEEP.value)


    def EmergencyStopUpdate(self):
        """ To stop updaing """
        return self.change_state(State.STOP.value)


    def GetAptListOutput(self):#TODO
        """ To stop updaing """
        return True


    # other class methods
    def error(self, err):
        """ usefull error method """
        self.lock.acquire()
        self.current_state = State.ERROR.value
        self.lock.release()
        self.error_message = err

        #reset fields
        self.update_file_name = ""
        self.update_file_path = ""

        self.Error(err) # send out error signal


    def update_failed(self, err):
        """ 
        Update failed reverting all part of update and clearing 
        working and update directories.
        """

        # goto error state
        self.error(err)

        # remove all updater files 
        shutil.rmtree(WORKING_DIR)
        shutil.rmtree(UPDATES_DIR)
        Path(WORKING_DIR).mkdir(parents=True, exist_ok=True)
        Path(UPDATES_DIR).mkdir(parents=True, exist_ok=True) 
        
        #reset fields not reset by error()
        self.available_updates = 0
        self.update_instruction = {}

        # TODO revert update


    def end(self):
        """ Use to stop all threads nicely """
        self.running = False
        self.change_state(State.STOP.value) # stop if currently updating
        if self.thread1.is_alive():
            self.thread1.join()


    def working_thread(self):
        while(self.running):
            if self.current_state == State.PREUPDATE.value:
                self.pre_update()
            elif self.current_state == State.UPDATE.value:
                self.update()
            else:
                time.sleep(1)


    def change_state(self, new_state):
        """
        State machine for linux updater anyt state change should 
        happen with this functionw with expection of errors. For errors,
        use the error() or updated_failed() methods as needed. 
        """

        self.lock.acquire()

        # Every state can transition to error state,
        # But errors should use error() or update_failed()
        if new_state == State.ERROR.value:
            self.current_state = State.ERROR.value
            self.lock.release()
            return True

        rv = False # return value, assume false by default

        # TODO make this better
        if self.current_state == State.ERROR.value:
            if self.current_state == Start.STOP.value or self.current_state == Start.Error.value:
                self.current_state = new_state
                rv = True
        elif self.current_state == State.SLEEP.value:
            if new_state == State.SLEEP.value or new_state == State.PREUPDATE.value:
                self.current_state = new_state
                rv = True
        elif self.current_state == State.PREUPDATE.value:
            if new_state == State.PREUPDATE.value or new_state == State.SLEEP.value or new_state == State.UPDATE.value:
                self.current_state = new_state
                rv = True
        elif self.current_state == State.UPDATE.value:
            if new_state == State.UPDATE.value or new_state == State.STOP.value or new_state == State.SLEEP.value:
                self.current_state = new_state
                rv = True
        elif self.current_state == State.STOP.value:
            if new_state == State.UPDATE.value or new_state == State.STOP.value:
                self.current_state = new_state
                rv = True
        else:
            self.errror("Unkown state")

        self.lock.release()
        return rv


    def pre_update(self):
        """ 
        Load oldest update file if one exist 
        Update thread function.
        Finds the oldest update file and starts update 
        """

        # see if any update file exist
        list_of_files = os.listdir(UPDATES_DIR)
        if not list_of_files:
            self.change_state(State.SLEEP.value)
            return # done, empty, no update files

        # check for valid update file
        self.update_file_name = list_of_files[0] # get 1st file
        self.update_file_path = UPDATES_DIR + self.update_file_name
        """
        if not re.match(r"(update-\d\d\d\d-\d\d-\d\d-\d\d-\d\d\.tar\.gz)", self.update_file_path):
            self.error("Not a valid update file")
            return
        """

        # copy file into working dir
        ret_path = shutil.copy(self.update_file_path, WORKING_DIR)
        if UPDATES_DIR in ret_path:
            self.updated_failed("Failed to copy into working dir")
            return

        # uncompress_file update file
        if not uncompress_file(ret_path):
            self.updated_failed("Untar failed")
            return

        # load instructins file
        try:
            with open(WORKING_DIR + "instructions.yml", "r") as file_object:
                # load instructions data into dictionary
                self.update_instruction = yaml.safe_load(file_object)
                if not self.update_instruction:
                    self.updated_failed("Not a valid yml file") # empty dictionary
                    return
        except IOError:
            self.updated_failed("Instructions file not accessible")
            return

        self.change_state(State.UPDATE.value)
        return


    def update(self):
        # install packages
        install_string = self.computer_name + "-install-pkgs"
        if install_string in self.update_instruction: # string key in dictionary
            pkgs = self.update_instruction[install_string]
            for p in pkgs:
                if not install(WORKING_DIR + p):
                    self.updated_failed("install pkg failed: " + p)
                    return

                if self.current_state == State.STOP.value:
                    return # stop update was called

        # remove packages
        remove_string = self.computer_name + "-remove-pkgs"
        if remove_string in self.update_instruction: # string key in dictionary
            pkgs = self.update_instruction[remove_string]
            for p in pkgs:
                if not remove(WORKING_DIR + p):
                    self.updated_failed("remove pkg failed: " + p)
                    return

                if self.current_state == State.STOP.value:
                    return # stop update was called

        # clear working dir and remove update deb pkg
        shutil.rmtree(WORKING_DIR)
        Path(WORKING_DIR).mkdir(parents=True, exist_ok=True)
        os.remove(self.update_file_path)
        self.update_file_name = ""
        self.update_file_path = ""
        self.available_updates -= 1
        self.update_instruction = {}
        self.change_state(State.SLEEP.value)
        return


def install(file_path):
    """ output will be 0 if it completes install, anything else fails """
    deb = apt.debfile.DebPackage(file_path)
    if deb.check() and deb.install() == 0: # valid package and install worked
        return True
    return False


def remove(file_path): # TODO change to python3-apt
    bashCommand = "sudo apt-get -qq remove ./"+ file_path
    output = subprocess.check_call(['bash','-c', bashCommand])
    return True


def uncompress_file(file_path): # TODO FLIF?
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
