<div align="center">
  <h1>
  <br> CANdaemon for OreSat </br>
  </h1>
  <h4>
    <a href="#features">Features</a> |
    <a href="#dependices">Dependices</a> |
    <a href="#subdir layout">Subdir Layout</a> |
    <a href="#settup">Settup</a>
  </h4>
</div>

The CANdaemon is [CANopenSocket], but with dbus interfaces to commicate with other process. Currently, the CANdaemon can commicate with main process ([GPS], [StarTracker], [OreSatLive], or the Cirrus Flux Camera process) and the Linux Updater.

## Features
- Follows the CiA (CAN-in-Automation) specs for [CANopen-Specs]. 
- Follows the [ECSS-CANBus-Extended-Protocal] on top of CiA specs.
- Designed to control other process over DBus from CAN.
- Easy to add more DBus clients to control new processes.

## Additions to CANopenSocket
- The Network Manager Node is now off;
- DBus interface (use sd-bus; systemd dbus system)
    - Has a extra thread for reading from data from the CAN Object Dictionary and interfacing with all other processes/daemons over DBus. 
    - For each process that the CANdaemon will interface with, there is a thread to read signals/properties from that dbus service and add that data to the CAN Object Dictionary.
    - Each app has a directory under src/ that contains the makefile, an unique Object Dictionary, and all Object Dictionary to DBus interface threads/functions.

## Dependices
- For beaglebone black: systemd-dev, make, gcc, pkg-source 

## Subdir Layout 
- **CANopenComm** - Source code for CANopenComm
- **CANopenNode** - git submodule for CANopenNode
- **common** - common source code for CANopenComm and CANdaemon
- **docs** - Documentation for CANdaemon
- **src** - Holds an CANdaemon app for each OreSat Linux board
- **useful_scipts** - Useful scripts like; the MDC image veiwer, enabling canable script, etc.

## Settup
- `git submodules init`
- `git submodules update`
- `cd src/<app folder>`
- `make clean candaemon`
- `./candaemon`

## Making a new app
- `cp -r src/app_template src/app_<new_app_name>`
- modify appilcation.* as needed

## Useful References
- [CANopenSocket]
- [DBus-Specs]
- [CANopen-Specs]
- [ECSS-CANBus-Extended-Protocal]

<!-- Other oresat repos -->
[GPS]:https://github.com/oresat/oresat-gps-software
[StarTracker]:https://github.com/oresat/oresat-star-tracker
[OreSatLive]:https://github.com/oresat/oresat-dxwifi-software

<!-- References --> 
[CANopenSocket]:https://github.com/CANopenNode/CANopenSocket
[DBus-Specs]:https://dbus.freedesktop.org/doc/dbus-specification.html
[CANopen-Specs]:https://www.can-cia.org/groups/specifications/
[ECSS-CANBus-Extended-Protocal]:https://ecss.nl/standard/ecss-e-st-50-15c-space-engineering-canbus-extension-protocol-1-may-2015/
