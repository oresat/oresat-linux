#!/usr/bin/env python3


import apt_pkg, apt.debfile
import subprocess


def install(file_path):
    """ output will be 0 if it completes install, anything else fails """
    deb = apt.debfile.DebPackage(file_path)
    if deb.check(): # valid package check
       return deb.install()
    else:
       return 1 


def remove(self, file_path): #TODO test the -q, test -qq
    #make sure the file contains the package name, not a deb
    bashCommand = "sudo apt-get -qq remove ./"+ file_path
    output = subprocess.check_call(['bash','-c', bashCommand])
    #print("remove")
    return output



