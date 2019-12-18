#!/usr/bin/env python3

"""
The updater file contains functions for installing updates for OreSat from
a .tar file. The primary function can be called using the following format. 

python3 updater.py updatefile.tar

The name of the updatefile.tar does not matter as long as it is a .tar file.
The updatefile.tar should contain one or more .deb files. This is the only
file type that will be installed by this function.

The function will have an associated, permament directory labled: old_updates. 
Once a .deb file has been installed, it will be moved to this directory to be 
saved. The .tar file and the tar directory will also be moved to this directory
once the process has been completed. 
"""


import os, sys, subprocess, apt_pkg, apt.debfile, re, yaml
from thread import Thread
from shutil import copy
from datetime import datetime


UPDATE_DIR = '/opt/oresat-linux-updater/updates/'


class State(Enum):
    ERROR = -1
    SLEEP = 0
    UPDATING = 1
    STOPPED = 2


class LinuxUpdater:
    computer_name = ""
    state = State.SLEEP.value
    t1 = Thread(target=update)


    def __init__():
	t1.daemon = True
        computer_name = "StarTracker" #TODO get computer name from dbus property


    def __del__(self):
        """ stop updater process """
        t1.join()


    def add_update_file(file_path):
        """ copies file into UPDATE_DIR """
        if(file_path[0] != '/')
            return False # not absolute path
        
        ret = copy(file_path, UPDATE_DIR)
        
        if UPDATE_DIR in ret:
            return True

        return False # failed to copy

    
    def update():
        """ find oldest update file and starts update """

        if self.state == State.UPDATING.value:
            return # currently updating

        self.state = State.UPDATING.value

        list_of_files = os.listdir(UPDATE_DIR)
        if not list_of_files:
            self.state = State.ERROR.value
            return 

        # check for valid update file
        file = list_of_files[0] # get 1st file
        m = re.match(r'linux-update-\d\d\d\d-\d\d-\d\d-\d\d-\d\d.tar.gz', file) #linux-update-2005-10-01-01-33.tar.gz (YY MM dd HH mm)
        if not m:
            self.state = State.ERROR.value
            return # not valid update file name

        tar_dir = untar(tar_file)
        if not tar_dir:
            self.state = State.ERROR.value
            return # untar failed

        try:
            with open(tar_dir + "/instruction.yml", "r") as file_object:
                #file_contents is a list of strings, each a line from txt file
                file_contents = file_object.readlines()
                ret_file = process_config_file(file_contents, tar_dir)
                prepare_tar(tar_file)
        except IOError:
            #print("File not accessible")
            self.state = State.ERROR.value
            return
        
        self.state = State.SLEEP.value


    def update_async():
        """ find oldest update file and starts update asynchronous """
        if not t1.isAlive():
            t1.start()


    def untar(self, file_name):
        """
        contents of .tar.gz file are extracted to a directory with the same
        name as the tar file file without the .tar.gz extension
        """
        
        bashCommand = "tar -xzf " + file_name
        output = subprocess.check_call(['bash','-c', bashCommand])

        #create a str with the directory name by slicing off the .tar.gz extension
        return file_name[0:len(file_name)-7]


    def install(self, file_path):
        """ output will be 0 if it completes install, anything else fails """
        deb = apt.debfile.DebPackage(file_path)
        if deb.check(): # valid package check
           return res = deb.install()
        else:
           return 1 


    def remove(self, file_path): #TODO test the -q, test -qq
        #make sure the file contains the package name, not a deb
        bashCommand = "sudo apt-get -qq remove ./"+ file_path
        output = subprocess.check_call(['bash','-c', bashCommand])
        #print("remove")
        return output


    def process_config_file(self, file_contents, dir_with_pkgs):
        data = yaml.safe_load(file_contents)

        if not data:
            return False

        # install packages
        pkgs = data[self.computer_name + "-install-pkgs"]
        for p in pkgs:
            r = install(dir_with_pkgs + p)
            if r != 0:
                self.update_error()
                return False

        # remove packages
        pkgs = data[self.computer_name + "-remove-pkgs"]
        for p in pkgs:
            r = remove(dir_with_pkgs + p)
            if r != 0:
                self.update_error()
                return False

        return True


    def update_error(self):
        self.state = State.ERROR.value
        #TODO undo installs and remmoves


    def prepare_tar(self, file_path):
        bashCommand = "tar -zcvf " + self.computer + "-completed.tar.gz ./" + file_path
        output = subprocess.check_call(['bash','-c', bashCommand])

