# OreSat Linux
This repo has all the general design and reasoning behind it for all the Linux boards on OreSat.
The current Linux boards being SDR GPS, Star Tracker, OreSat Live, and Cirrus Flux Camera (CFC).


## Why Linux?
Some of our systems need more computing power, so they're given a [Sitara AM335x] that can run Linux.


## General Board Design
All A8 onboard OreSat will all have 3 daemons; the candaemon, the Linux updater, and the main process. 
![](https://github.com/oresat/oresat-linux-candaemon/blob/master/docs/OreSatLinuxDiagram.jpg)

### [oresat-linux-candaemon]
OreSat uses [CAN] for commucation between systems onboard and follows the [CANopen] specifications.
The candaemon acts a front end for all of OreSat Linux daemons and is build on top of [CANopenNode]. 
It allows the [C3], Oresat' CAN Network Manger, to control or get data from daemons on the Linux board. 
It will use [DBus] to inter-process communication with daemons. 
Therefor the main projects can be implemented in any programing language that has [DBus] library 
and not worry about finding / making a CAN library following [CANopen] specifications for that language.
See the [oresat-linux-candaemon] repo for more info.

### [oresat-linux-updater]
A daemon that allows the Linux board to be updated over dbus.
See the [oresat-linux-updater] repo for more info.

### Main Process Repo
| Project               | Hardware                  | Software                          |
|-----------------------|---------------------------|-----------------------------------|
| SDR GPS               | [oresat-gps-hardware]     | [oresat-gps-software]             |
| Star Tracker          | [oresat-star-tracker]     | [oresat-star-tracker-software]    |
| OreSat Live           | [oresat-dxwifi-hardware]  | [oresat-dxwifi-software]          |
| Cirrus Flux Camera    | TBD                       | TBD                               |



<!-- OreSat repos -->
[oresat-gps-software]:https://github.com/oresat/oresat-gps-software
[oresat-gps-hardware]:https://github.com/oresat/oresat-gps-hardware
[oresat-star-tracker]:https://github.com/oresat/oresat-star-tracker
[oresat-star-tracker-software]:https://github.com/oresat/oresat-star-tracker-software
[oresat-dxwifi-hardware]:https://github.com/oresat/oresat-dxwifi-hardware
[oresat-dxwifi-software]:https://github.com/oresat/oresat-dxwifi-software
[oresat-linux-candaemon]:https://github.com/oresat/oresat-linux-candaemon
[oresat-linux-updater]:https://github.com/oresat/oresat-linux-updater
[C3]:https://github.com/oresat/oresat-c3

<!-- Other repos -->
[CANopenNode]:https://github.com/CANopenNode/CANopenNode

<!-- Other links -->
[CAN]:https://en.wikipedia.org/wiki/CAN_bus
[CANopen]:https://en.wikipedia.org/wiki/CANopen
[DBus]:https://en.wikipedia.org/wiki/D-Bus
[Sitara AM335x]:https://www.ti.com/processors/sitara-arm/am335x-cortex-a8/overview.html
