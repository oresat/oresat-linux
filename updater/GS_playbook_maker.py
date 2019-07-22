#!/usr/bin/env python3

import pathlib

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


output = readPkgFile("required_pkgs.txt")
makeTaskFile(output)
for i in output:
    print(i)
