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



import os, sys, subprocess
ARGS = 3 #how many args are expected


def main():
    if(len(sys.argv) == ARGS+1):
        file_read(sys.argv[1], sys.argv[2], sys.argv[3])
    else:
        print("Error: updater.py function called with incorrect # of args")


if __name__=="__main__":
    main()

