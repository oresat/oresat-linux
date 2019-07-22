# OreSat Linux Updater
These script will allow OreSat A8 to be updated when it is in space.

## Ground Station side
The ground station side will used ansible and QEMU to make a tar of packages, source code, a instruction yaml file. QEMU will have a virtual copy of 4 A8 on OreSat (GPS, Star Tracker, OreSat Live, & Cirrus Flux Camera) for ansible to controll. 

## OreSat side
The onboard OreSat side will untar the tar of packages and install the packages listed in instuction for the A8 it is currently running on.
