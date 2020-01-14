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

The CANdaemon is [CANopenSocket], but with dbus interfaces to commicate with other process. The CANdaemon can commicate with main process ([GPS], [StarTracker], [OreSatLive], or the Cirrus Flux Camera process) and the Linux Updater. The CANdaemon not ment to be a Node on the CANbus, not the Network Manager.

![](https://github.com/oresat/oresat-linux/blob/master/CANdaemon/docs/OreSatLinuxDiagram.jpg)

## Features
- Follows the CiA (CAN-in-Automation) specs for [CANopen-Specs]. 
- Follows the [ECSS-CANBus-Extended-Protocal] on top of CiA specs.
- Designed to control other process over DBus from CAN using sd-bus (systemd dbus system).
- Easy to add more DBus clients to control new processes over CAN.

## Subdir Layout 
- **boards** - Holds an CANdaemon app for each OreSat Linux board
- **CANopenNode** - git submodule for CANopenNode
- **common** - common source code for CANopenComm and CANdaemon
- **docs** - Documentation for CANdaemon

## Dependices
- For Debian: libsystemd-dev, make, gcc, pkg-source 
- For Arch: make, gcc, pkg-source 

## How to use
### Setup
- `git submodules init`
- `git submodules update`

### Running CANdaemon
- `make <board>`
- `make config` if you want to edit the config file
- `make candaemon`

### Making a new board
- `cp -r boards/template boards/<new_board_name>`
- modify /boards/<new_board_name>/appilcation.* as needed
- modify /boards/<new_board_name>/objDict with [libedssharp] as needed

## Useful References
- [CAN-Wikipedia]
- [CANopenSocket]
- [DBus-Specs]
- [CANopen-Specs]
- [ECSS-CANBus-Extended-Protocal]

<!-- Other oresat repos -->
[GPS]:https://github.com/oresat/oresat-gps-software
[StarTracker]:https://github.com/oresat/oresat-star-tracker
[OreSatLive]:https://github.com/oresat/oresat-dxwifi-software

<!-- References --> 
[CAN-Wikipedia}:https://en.wikipedia.org/wiki/CAN_bus
[CANopenSocket]:https://github.com/CANopenNode/CANopenSocket
[DBus-Specs]:https://dbus.freedesktop.org/doc/dbus-specification.html
[CANopen-Specs]:https://www.can-cia.org/groups/specifications/
[ECSS-CANBus-Extended-Protocal]:https://ecss.nl/standard/ecss-e-st-50-15c-space-engineering-canbus-extension-protocol-1-may-2015/

<!-- Other --> 
[libedssharp]:https://github.com/robincornelius/libedssharp
