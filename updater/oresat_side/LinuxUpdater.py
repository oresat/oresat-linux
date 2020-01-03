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
from shutil import copy
from datetime import datetime
from pathlib import Path
from pydbus.generic import signal
from pydbus import SystemBus
from gi.repository import GLib
from enum import Enum
import os, sys, re, yaml
import LinuxUpdaterDeb
import LinuxUpdaterTar


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
            <property name="ComputerName" type="s" access="read"/>
            <property name="CurrentUpdateFile" type="s" access="read"/>
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
        self.thread1 = Thread(target=self.update, name="UpdateThread")
        # make directories for updater, if not found
        Path(UPDATES_DIR).mkdir(parents=True, exist_ok=True) 
        Path(WORKING_DIR).mkdir(parents=True, exist_ok=True)


    def __del__(self):
        """ stop updater process """
        if self.current_state == State.UPDATE.value:
            self.current_state = State.SLEEP.value
        if self.thread1.isAlive():
            self.thread1.join()


    # signals
    Error = signal()
    UpdateDone = signal()


    # properties
    @property
    def Status(self):
        return self.current_state


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
        
        ret = copy(file_path, UPDATES_DIR)
        
        if UPDATES_DIR in ret:
            return True
        
        return False # failed to copy


    def StartUpdate(self):
        """ To start updaing process with file_path """
        if self.current_state == State.SLEEP.value or self.current_state == State.STOP.value:
            if thread1.isAlive():
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
        self.Error(err) # send out error signal


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
            return None # done, no update files

        # check for valid update file
        file = list_of_files[0] # get 1st file
        if not re.match(r'linux-update-\d\d\d\d-\d\d-\d\d-\d\d-\d\d.tar.gz', file):
            self.error("not a valid update file")
            return None
        if update_file[0] != '/':
            self.error("not absoult path")
            return None

        # copy file into working dir
        ret = copy(update_file, WORKING_DIR)
        if UPDATES_DIR in ret:
            self.error("failed to copy into working dir")
            return None

        # untar update file
        tar_dir = untar(ret)
        if not tar_dir:
            self.error("untar failed")
            return None

        # load instructins file
        try:
            with open(tar_dir + "/instruction.yml", "r") as file_object:
                # file_contents is a list of strings, each a line from txt file
                file_contents = file_object.readlines()
        except IOError:
            self.error("instructions file not accessible")
            return None

        # change instructions data into dictionary
        intructions_dict = yaml.safe_load(file_contents)
        if not intructions_dict:
            self.error("Not a valid yml file")
            return None

        # process config file 
        ret_file_data = self.process_config_file(intructions_dict, tar_dir)

        # save output dictionary to file
        try:
            with open(tar_dir + '/update.yml', 'w') as new_file:
                yaml.dump(ret_file_data, new_yml_file)
        except IOError:
            self.error("return file data format error")
            return None
        
        # tar output file
        ret_file_file = prepare_tar(new_yml_file)

        # send file to CANdaemon
        # send(ret_tar_file)

        # clean working dir
        shutil.rmtree(WORKING_DIR) 

        self.current_state = State.SLEEP.value
        return ret_tar_file


    def process_config_file(self, intruction_dicts, dir_with_pkgs):
        return_dict = {}

        # install packages
        pkgs = intructions_dict[self.computer_name + "-install-pkgs"]
        for p in pkgs:
            r = install(dir_with_pkgs + p)
            if r != 0:
                self.error = "not an absolute path: " + p
                p = p + " error"
                break
            else:
                p = p + " installed"
                
            if self.running == False:
                break # stop update was called

        return_dict[self.computer_name + "-install-pkgs"] = pkgs
        
        if self.running == False or not self.err:
            # stop update was called or error occured
            return return_dict

        # remove packages
        pkgs = intructions_dict[self.computer_name + "-remove-pkgs"]
        for p in pkgs:
            r = remove(dir_with_pkgs + p)
            if r != 0:
                self.error = "not an absolute path: " + p
                p = p + " error"
                break
            else:
                p = p + " installed"
        
            if self.running == False:
                break # stop update was called
        
        return_dict[self.computer_name + "-remove-pkgs"] = pkgs

        return return_dict


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

