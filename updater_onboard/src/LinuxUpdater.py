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

from threading import Thread
import shutil
from datetime import datetime
from pathlib import Path
from pydbus.generic import signal
from pydbus import SystemBus
from gi.repository import GLib
from enum import Enum
import os, sys, re, yaml, subprocess
import apt_pkg, apt.debfile
import LinuxUpdaterDeb


INTERFACE_NAME = "org.OreSat.Updater"
UPDATES_DIR = '/tmp/oresat-linux-updater/updates/'
WORKING_DIR = '/tmp/oresat-linux-updater/working/'


class State(Enum):
    ERROR = -1
    SLEEP = 0
    PREUPDATE = 1
    UPDATE = 2
    STOP = 3


class LinuxUpdater(object):
    dbus = """
    <node>
        <interface name="org.OreSat.Updater">
            <signal name="Error">
                <arg type='s'/>
            </signal>
            <signal name="UpdateDone">
                <arg type='s'/>
            </signal>
            <property name="Status" type="d" access="read"/>
            <property name="ComputerName" type="s" access="readwrite"/>
            <property name="CurrentUpdateFile" type="s" access="read"/>
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
        </interface>
    </node>
    """

    def __init__(self):
        # set local fields
        self.current_state = State.SLEEP.value
        self.computer_name = "StarTracker"
        self.error_message = ""
        self.running = True
        self.update_file_name = ""
        self.update_instruction = {}
        self.thread1 = Thread(target=self.update, name="working_thread")

        # start working thread
        self.thread1.start()

        # make directories for updater, if not found
        Path(UPDATES_DIR).mkdir(parents=True, exist_ok=True) 
        Path(WORKING_DIR).mkdir(parents=True, exist_ok=True)


    def __del__(self):
        """ stop updater process """
        self.running = False
        self.change_state(State.STOP.value) # stop if currently updating
        if self.thread1.is_alive():
            self.thread1.join()


    # signals
    Error = signal()
    UpdateDone = signal()


    # properties
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


    # methods
    def AddUpdateFile(self, file_path):
        """ copies file into UPDATES_DIR """
        if(file_path[0] != '/'):
            self.error("not an absolute path: " + file_path)
            return False
        
        ret = shutil.copy(file_path, UPDATES_DIR)
        
        if UPDATES_DIR in ret:
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


    # other class methods
    def error(self, err):
        """ usefull error method """
        self.current_state = State.ERROR.value
        self.error_message = err
        self.update_file_name = ""
        self.Error(err) # send out error signal


    def update_failed(self, err):
        """ 
        Update failed reverting all part of update and clearing 
        working and update directories.
        """

        # goto error state
        self.error(err)

        #reset fields
        self.update_file_path = ""
        self.update_file_name = ""
        self.update_instruction = {}

        # remove all updater files
        shutil.rmtree(WORKING_DIR)
        shutil.rmtree(UPDATES_DIR)

        # TODO revert update


    def working_thread(self):
        while(self.running):
            if self.current_state ==  State.PREUPDATE.value:
                self.pre_update()
            elif self.current_state == State.UPDATE.value:
                self.update()
            else:
                sleep(1)


    def pre_update(self):
        """ 
        Update thread function.
        Finds the oldest update file and starts update 
        """

        # see if any update file exist
        list_of_files = os.listdir(UPDATES_DIR)
        if not list_of_files:
            self.current_state = State.SLEEP.value
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
            self.updateFailed("Failed to copy into working dir")
            return

        # untar update file
        if not untar(ret_path):
            self.updateFailed("Untar failed")
            return

        # load instructins file
        try:
            with open(WORKING_DIR + "instructions.yml", "r") as file_object:
                # load instructions data into dictionary
                self.update_instruction = yaml.safe_load(file_object)
                if not self.update_instruction:
                    self.updateFailed("Not a valid yml file") # empty dictionary
                    return
        except IOError:
            self.updateFailed("Instructions file not accessible")
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
                    self.updateFailed("install pkg failed: " + p)
                    return

                if self.current_state == State.STOP.value:
                    return # stop update was called

        # remove packages
        remove_string = self.computer_name + "-remove-pkgs"
        if remove_string in self.update_instruction: # string key in dictionary
            pkgs = self.update_instruction[remove_string]
            for p in pkgs:
                if not remove(WORKING_DIR + p):
                    self.updateFailed("remove pkg failed: " + p)
                    return

                if self.current_state == State.STOP.value:
                    return # stop update was called

        # clear working dir and remove update pkg
        shutil.rmtree(WORKING_DIR)
        os.remove(self.update_file_path)
        self.update_file_name = ""
        self.update_file_path = ""
        self.update_instruction = {}
        self.change_state(State.SLEEP.value)
        return


    def change_state(self, new_state):
        """
        State machine for linux updater anyt state change should 
        happen with this functionw with expection of errors. For errors,
        use the error() or updated_failed() methods as needed. 
        """

        threadLock.acquire()

        # every state can transition to error state
        if new_state = ERROR:
            self.error("change state function switch to error state")
            threadLock.release()
            return True

        rv = False # return value, assume false by default

        if self.current_state == State.ERROR.value:
            if self.current_state == Start.STOP.value or self.current_state == Start.Error.value:
                rv = True
        elif self.current_state == State.SLEEP.value:
            if new_state == State.SLEEP.value or new_state == State.PREUPDATE.value:
                self.current_state = new_state:
                rv = True
        elif self.current_state == State.PREUPDATE.value:
            if new_state == State.PREUPDATE.value or new_state == State.UPDATE.value:
                self.current_state = new_state:
                rv = True
        elif self.current_state == State.UPDATE.value:
            if new_state == State.UPDATE.value or new_state == State.STOP.value:
                self.current_state = new_state:
                rv = True
        elif self.current_state == State.STOP.value:
            if new_state == State.UPDATE.value or new_state == State.STOP.value:
                self.current_state = new_state:
                rv = True
        else
            self.errror("Unkown state")

        ThreadLock.release()
        return rv


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


def untar(file_path):
    """
    contents of .tar.gz file are extracted to a directory with the same
    name as the tar file file without the .tar.gz extension
    """
    
    bashCommand = "tar -xzf " + file_path + " -C " + WORKING_DIR
    output = subprocess.check_call(['bash','-c', bashCommand])

    #create a str with the directory name by slicing off the .tar.gz extension
    return True # file_name[0:len(file_path)-7]


#startLinuxUpdater():
bus = SystemBus()
loop = GLib.MainLoop()
emit = LinuxUpdater()
bus.publish(INTERFACE_NAME, emit)

try:
    loop.run()
except KeyboardInterrupt as e:
    loop.quit()
    print("\nExit by Control C")

