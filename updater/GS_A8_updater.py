#!/usr/bin/env python3

import subprocess
import pathlib
import sys
import re

USER="debian"
HOST="192.168.7.2"
KEYPATH="~/.ssh/oresat"
PLAYBOOK_PATH="./example_playbook/playbook.yml"


def getInstalledPkgsList():
    pkgs = []

    ssh = subprocess.Popen(["ssh", "-i", KEYPATH,  USER + '@' + HOST, "apt list --installed"],
                            shell=False,
                            stdout=subprocess.PIPE,
                            stderr=subprocess.PIPE)

    result = ssh.stdout.readlines()
    for i in result:
        temp = i.decode('ascii')
        pkgs.append(temp.rstrip())

    return pkgs

def getListDifference(installList, newList):
    removeList = installList.copy()
    addList = newList.copy()
    for i in installList:
        if (i in newList) and (i in installList):
            removeList.remove(i)
            addList.remove(i)

    return removeList, addList

def readPkgFile(file):
    pkgs = []

    with open(file, "r") as f:
        for line in f:
            pkgs.append(line.rstrip())
    
    return pkgs

def makeTaskFile(pkgsList):
    pathlib.Path("roles/basic/tasks").mkdir(parents=True, exist_ok=True)
    with open("roles/basic/tasks/main.yml", "w") as f:
        f.write("- name: Install new packages\n")
        f.write("  apt: name={{item}} state=present\n")
        f.write("  with_items:\n")
        for i in pkgsList:
            f.write("    - %s\n" % i)



def main():
    print("Getting list of currently install packages")
    results = getInstalledPkgsList()

    print("Reading required pkgs file")
    output = readPkgFile("required_pkgs.txt")
    if (output == []):
        print("No packages ot update")
        return -1

    print("Making a task & playbook for Ansible")
    makeTaskFile(output)
    print("Running Ansible")
    output2 = subprocess.Popen(["ansible-playbook", "-K", PLAYBOOK_PATH],
                                shell=False,
                                stdout=subprocess.PIPE,
                                stderr=subprocess.PIPE)
    #print(output2)

    print("Getting list of currently install packages")
    results2 = getInstalledPkgsList()

    print("Getting difference")
    removeList, addList = getListDifference(results, results2)

    print("Removing: %s" % removeList)
    print("Adding: %s" % addList)

    #make custom yaml file
    return 0

if __name__ == "__main__":
    main()
