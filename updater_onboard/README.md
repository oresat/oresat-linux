# OreSat Linux Updater (onboard)
The onboard OreSat side will untar the tar of packages and install the packages listed in instuctions.conf for the A8 it is currently running on.

## The Dbus interface
![](https://github.com/oresat/oresat-linux/blob/master/updater/oresat_side/LinuxUpdaterDbusInterface.jpg)
The Linux updater has 2 dbus methods for control; one to start an update and one to stop an update. It will always be sending a signal of it's current status.
