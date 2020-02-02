<div align="center">
  <h1>
    <br> Oresat Linux CANdaemon </br>
  </h1>
  <h4>
    <a href="#features">Features</a> |
    <a href="#directory layout">Directory Layout</a> |
    <a href="#dependices">Dependices</a> |
    <a href="#how to use">How to use</a>
  </h4>
</div>

The CANdaemon is based off of [CANopenSocket], but with dbus interfaces to commicate and control daemons. The CANdaemon can commicate with main process ([GPS], [StarTracker], [OreSatLive], or the Cirrus Flux Camera daemons) and the Linux Updater. The CANdaemon is ment to be a node on the CANbus, not the Network Manager.

![](https://github.com/oresat/oresat-linux/blob/master/CANdaemon/docs/OreSatLinuxDiagram.jpg)

## Features
- Follows the CiA (CAN-in-Automation) specs for [CANopen-Specs]. 
- Follows the [ECSS-CANBus-Extended-Protocal] on top of CiA specs.
- Designed to allow the CAN Network Manager to have control of local daemons.
- Uses sd-bus for dbus communication (systemd dbus) to daemons.
- Easy to add more DBus clients to control new daemons over CAN.

## Directory Layout 
- **boards** - Holds an CANdaemon app for each OreSat Linux board
- **CANopenNode** - The git submodule for CANopenNode (backend of CANdaemon)
- **common** - Common source code for CANdaemon, regardless of which app is enabled.
- **docs** - Documentation for CANdaemon

## Dependices
- For Debian: libsystemd-dev, cmake, make, gcc
    - optional: ninja-build
- For Arch: cmake, make, gcc
    - optional: ninja

## How to use
### Setup
- `git submodules init`
- `git submodules update`

### Running CANdaemon
- Using cmake and make
    - `cmake .`
    - `make all`
    - `./candaemon`
- Using cmake and ninja
    - `cmake -GNinja .`
    - `ninja`
    - `./candaemon`
- Optional flags:
    - `cmake -DCMAKE_BUILD_TYPE=Debug .` to turn on -g, -Wall, and -Wextra flags
- Installing binary and daemon service file (usefull for testing)
    - `sudo make install` or `sudo ninja install`
- Building deb binary package
    - On a beaglebone (or debian based armhf system) `sudo make package`

### Making a new board
- `cp -r boards/template boards/<new_board_name>`
- modify /boards/<new_board_name>/appilcation.* as needed
- modify /boards/<new_board_name>/objDict with [libedssharp] as needed

## Useful References
- [CAN-Wikipedia]
- [CANopenSocket]
- [Systemd-DBus]
- [DBus-Specs]
- [CANopen-Specs]
- [ECSS-CANBus-Extended-Protocal]

<!-- Other oresat repos -->
[GPS]:https://github.com/oresat/oresat-gps-software
[StarTracker]:https://github.com/oresat/oresat-star-tracker
[OreSatLive]:https://github.com/oresat/oresat-dxwifi-software

<!-- References -->
[CAN-Wikipedia]:https://en.wikipedia.org/wiki/CAN_bus
[CANopenSocket]:https://github.com/CANopenNode/CANopenSocket
[Systemd-DBus]:https://www.freedesktop.org/wiki/Software/systemd/dbus/
[DBus-Specs]:https://dbus.freedesktop.org/doc/dbus-specification.html
[CANopen-Specs]:https://www.can-cia.org/groups/specifications/
[ECSS-CANBus-Extended-Protocal]:https://ecss.nl/standard/ecss-e-st-50-15c-space-engineering-canbus-extension-protocol-1-may-2015/

<!-- Other --> 
[libedssharp]:https://github.com/robincornelius/libedssharp
