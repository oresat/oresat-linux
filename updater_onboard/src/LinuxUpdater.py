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
    UPDATE = 1
    STOP = 2


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
            <method name='EmergencyStopUpdate'>
                <arg type='b' name='output' direction='out'/>
            </method>
        </interface>
    </node>
    """

    def __init__(self):
        self.current_state = State.SLEEP.value
        self.computer_name = "StarTracker"
        self.current_update_file = ""
        self.error_message = ""
        self.thread1 = Thread(target=self.update, name="UpdateThread")
        # make directories for updater, if not found
        Path(UPDATES_DIR).mkdir(parents=True, exist_ok=True) 
        Path(WORKING_DIR).mkdir(parents=True, exist_ok=True)


    def __del__(self):
        """ stop updater process """
        if self.current_state == State.UPDATE.value:
            self.current_state = State.SLEEP.value
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
        return self.current_update_file


    # methods
    def AddUpdateFile(self, file_path):
        """ copies file into UPDATES_DIR """
        if(file_path[0] != '/'):
            self.error = "not an absolute path: " + file_path
            return False
        
        ret = shutil.copy(file_path, UPDATES_DIR)
        
        if UPDATES_DIR in ret:
            return True
        
        return False # failed to copy


    def StartUpdate(self):
        """ To start updaing process with file_path """
        if self.current_state == State.SLEEP.value or self.current_state == State.STOP.value:
            if self.thread1.is_alive():
                self.error("Update thread already running, not in update state")
            else:
                self.thread1.start()
            return True
        elif self.current_state == State.UPDATE.value:
            return True # already running

        return False


    def StopUpdate(self):
        if self.current_state == State.UPDATE.value:
            self.current_state = State.STOP.value
            return True
        self.error("Update thread not running. Nothing to stop.")
        return False

    # other class methods
    def error(self, err):
        self.current_state = State.ERROR.value
        self.error_message = err
        self.Error(err) # send out error signal

    def updateFailed(self, err):
        """ 
        Update failed reverting all part of update and clearing 
        working and update directories
        """
        self.error(err)
        shutil.rmtree(WORKING_DIR)
        shutil.rmtree(UPDATES_DIR)

        # revert update


    def update(self):
        """ 
        Update thread function.
        Finds the oldest update file and starts update 
        """
        self.current_state = State.UPDATE.value

        # see if any update file exist
        list_of_files = os.listdir(UPDATES_DIR)
        if not list_of_files:
            self.current_state = State.SLEEP.value
            return # done, empty, no update files

        # check for valid update file
        self.current_update_file = list_of_files[0] # get 1st file
        update_file_path = UPDATES_DIR + self.current_update_file
        """
        if not re.match(r"(update-\d\d\d\d-\d\d-\d\d-\d\d-\d\d\.tar\.gz)", update_file_path):
            self.error("Not a valid update file")
            return
        """

        # copy file into working dir
        ret_path = shutil.copy(update_file_path, WORKING_DIR)
        if UPDATES_DIR in ret_path:
            self.updateFailed("Failed to copy into working dir")
            return

        # untar update file
        if not untar(ret_path):
            self.updateFailed("Untar failed")
            return

        # load instructins file
        instruction_dict = {}
        try:
            with open(WORKING_DIR + "instructions.yml", "r") as file_object:
                # file_contents is a list of strings, each a line from txt file
                #file_contents = file_object.readlines()
                # change instructions data into dictionary
                instruction_dict = yaml.safe_load(file_object)
                if not instruction_dict:
                    self.updateFailed("Not a valid yml file")
                    return
        except IOError:
            self.updateFailed("Instructions file not accessible")
            return

        # install packages
        install_string = self.computer_name + "-install-pkgs"
        if install_string in instruction_dict: # string key in dictionary
            pkgs = instruction_dict[install_string]
            for p in pkgs:
                if not install(WORKING_DIR + p):
                    self.updateFailed("install pkg failed: " + p)
                    return
                    
                if self.current_state == State.STOP.value:
                    return # stop update was called

        # remove packages
        remove_string = self.computer_name + "-remove-pkgs"
        if remove_string in instruction_dict: # string key in dictionary
            pkgs = instruction_dict[remove_string]
            for p in pkgs:
                if not remove(WORKING_DIR + p):
                    self.updateFailed("remove pkg failed: " + p)
                    return
            
                if self.current_state == State.STOP.value:
                    return # stop update was called

        # clear working dir and remove update pkg
        shutil.rmtree(WORKING_DIR)
        os.remove(update_file_path)
        self.current_state = State.SLEEP.value
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

