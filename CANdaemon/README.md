<div align="center">
  <h1>
  <br> CANdaemon for OreSat </br>
  </h1>
  <h4>
    <a href="#features">Features</a> |
    <a href="../BBB_Notes.md">BBB Notes</a>
  </h4>
</div>

The CANdaemon is [CANopenSocket], but with dbus interfaces to commicate with other process. Currently, the CANdaemon can commicate with main process ([GPS], [StarTracker], OreSat Live, or the Cirrus Flux Camera process) and the Linux Updater.

## Features
- Follows the CiA (CAN-in-Automation) specs for [CANopen-Specs]. 
- Follows the [ECSS-CANBus-Extended-Protocal] on top of CiA specs.
- Designed to control other process over DBus from CAN.
- Easy to add more DBus clients to control new processes.

## Additions to CANopenSocket
- The Network Manager Node is now a option; normally turn off now, but for testing, it can be useful to turn on.
- DBus interface (use sd-bus; systemd dbus system)
    - Has a extra thread for reading from data from the CAN Object Dictionary and interfacing with all other processes/daemons over DBus. 
    - For each process that the CANdaemon will interface with, there is a thread to read signals/properties from that dbus service and add that data to the CAN Object Dictionary.
- Multiple compile options for having multiple different versions; like a GPS version, StarTracker version, etc.
    - Each version has a directory under src/ that contains an unique Object Dictionary and all Object Dictionary to DBus interface threads/functions.

## Dependices (for beaglebone black)
- systemd-dev, make, gcc, pkg-source

## Useful References
- [CANopenSocket]
- [DBus-Specs]
- [CANopen-Specs]
- [ECSS-CANBus-Extended-Protocal]

<!-- Other oresat repos -->
[GPS]:https://github.com/oresat/oresat-gps-software
[StarTracker]:https://github.com/oresat/oresat-star-tracker

<!-- References --> 
[CANopenSocket]:https://github.com/CANopenNode/CANopenSocket
[DBus-Specs]:https://dbus.freedesktop.org/doc/dbus-specification.html
[CANopen-Specs]:https://www.can-cia.org/groups/specifications/
[ECSS-CANBus-Extended-Protocal]:https://ecss.nl/standard/ecss-e-st-50-15c-space-engineering-canbus-extension-protocol-1-may-2015/
