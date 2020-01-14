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

![](https://github.com/oresat/oresat-linux/blob/master/CANdaemon/docs/OreSatLinuxDiagram.jpg)

## Features
- Follows the CiA (CAN-in-Automation) specs for [CANopen-Specs]. 
- Follows the [ECSS-CANBus-Extended-Protocal] on top of CiA specs.
- Designed to control other process over DBus from CAN.
- Easy to add more DBus clients to control new processes.

## Changes to CANopenSocket
- The CANdaemon cannot be a Network Manager Node
- DBus interface (use sd-bus; systemd dbus system)
    - Has a extra thread for reading from data from the CAN Object Dictionary and interfacing with all other processes/daemons over DBus. 
    - For each process that the CANdaemon will interface with, there is a thread to read signals/properties from that dbus service and add that data to the CAN Object Dictionary.
    - Each board has a directory under boards/ that contains an unique Object Dictionary and all Object Dictionary to DBus interface threads/functions.

## Dependices
- For beaglebone black: libsystemd-dev, make, gcc, pkg-source 

## Subdir Layout 
- **CANopenComm** - Source code for CANopenComm
- **CANopenNode** - git submodule for CANopenNode
- **common** - common source code for CANopenComm and CANdaemon
- **docs** - Documentation for CANdaemon
- **boards** - Holds an CANdaemon app for each OreSat Linux board

## Settup
- `git submodules init`
- `git submodules update`
- `make <board>`
- `make config` if you want to edit the config file
- `make candaemon`

## Making a new board
- `cp -r boards/template boards/<new_board_name>`
- modify /boards/<new_board_name>/appilcation.* as needed
- modify /boards/<new_board_name>/objDict with [libedssharp] as needed

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

<!-- Other --> 
[libedssharp]:https://github.com/robincornelius/libedssharp
