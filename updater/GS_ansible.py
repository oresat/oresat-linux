#!/usr/bin/env python3

import subprocess
import pathlib
import sys
import re

USER="debian"
KEYPATH="~/.ssh/oresat"
PLAYBOOK_PATH="playbook.yml"

GPS_ADDRESS="192.168.7.2"
ST_ADDRESS="192.168.7.2"
CFC_ADDRESS="192.168.7.2"
OL_ADDRESS="192.168.7.2"

GPS_FILEPATH="gps_required.txt"
ST_FILEPATH="st_required.txt"
CFC_FILEPATH="cfc_required.txt"
OL_FILEPATH="ol_required.txt"

def makePlaybook(pkgsList):
    # clean last playbook 
    with open(PLAYBOOK_PATH, "w") as f:
        f.write("---\n")

    GPSpkgs = readPkgFile(GPSfile)
    STpkgs = readPkgFile(STfile)
    CFCpkgs = readPkgFile(CFCfile)
    OLpkgs = readPkgFile(OLfile)
    addToPlaybook(GPSpkgs, "GPS")
    addToPlaybook(STpkgs, "StarTracker")
    addToPlaybook(CFCpkgs, "CirrusFluxCamera")
    addToPlaybook(OLpkgs, "OreSatLive")
    return 1

def addToPlaybook(pkgsList, hostname):
    if(pkgsList == []):
        return 0

    with open(PLAYBOOK_PATH, "w+") as f:
        f.write("\n")
        f.write("- hosts: %s\n" % hostname)
        f.write("  remote_user: debian\n")
        f.write("\n")
        f.write("  tasks:\n")
        f.write("  - name: Install packages\n")
        f.write("    apt:\n")
        f.write("      name: \"{{item}}\"\n")
        f.write("      state: present\n")
        f.write("      with_items:\n")
        for i in pkgsList:
            f.write("        - %s\n" % i)

    return 1

def runAnsible():
    print("Making playbook for Ansible")
    makePlaybook()

    print("Running Ansible")
    AnsibleOutput = subprocess.Popen(["ansible-playbook", "-K", PLAYBOOK_PATH],
                                shell=False,
                                stdout=subprocess.PIPE,
                                stderr=subprocess.PIPE)

    result = AnsibleOutput.stdout.readlines()

    #save results to log file
    
    print("Ansible Output: %s" % result)

    return 0

runAnsible():
