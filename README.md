# OreSat Linux
This repo has all the general design and reasoning behind it for all the Linux boards on OreSat.
The current Linux boards being SDR GPS, Star Tracker, OreSat Live, and Cirrus Flux Camera (CFC).


## Why Linux?
Some of our systems need more computing power, so they're given a [Sitara AM335x] that can run Linux.


## General Board Design
All A8 onboard OreSat will all have 3 daemons; the candaemon, the Linux updater, and the main process. 
![](https://github.com/oresat/oresat-linux-candaemon/docs/OreSatLinuxDiagram.jpg)

### candaemon
OreSat uses [CAN] for commucation between systems onboard and follows the [CANopen] specifications.
The candaemon acts a front end for all of OreSat Linux daemons and is build on top of [CANopenNode]. 
It allows the [C3], Oresat' CAN Network Manger, to control or get data from daemons on the Linux board. 
It will use [DBus] to inter-process communication with daemons. 
Therefor the main projects can be implemented in any programing language that has [DBus] library 
and not worry about finding / making a CAN library following [CANopen] specifications for that language.
See the [candaemon] repo for more info.

### Linux updater
A daemon that allows the Linux board to be updated over dbus.
See the [Linux updater] repo for more info.

### Main Process
| Project               | Hardware  | Software  |
| --------------------- |:---------:| :-------: |
| SDR GPS               | [GPS HW]  | [GPS SW]  |
| Star Tracker          | [ST HW]   | [ST SW]   |
| OreSat Live           | [Live HW] | [Live SW] |
| Cirrus Flux Camera    | TBD       | TBD       |



<!-- OreSat repos -->
[GPS SW]:https://github.com/oresat/oresat-gps-software
[GPS HW]:https://github.com/oresat/oresat-gps-hardware
[ST HW]:https://github.com/oresat/oresat-star-tracker
[ST SW]:https://github.com/oresat/oresat-star-tracker-software
[Live HW]:https://github.com/oresat/oresat-dxwifi-hardware
[Live SW]:https://github.com/oresat/oresat-dxwifi-software
[candaemon]:https://github.com/oresat/oresat-linux-candaemon
[Linux updater]:https://github.com/oresat/oresat-linux-updater
[C3]:https://github.com/oresat/oresat-c3

<!-- Other repos -->
[CANopenNode]:https://github.com/CANopenNode/CANopenNode

<!-- Other links -->
[CAN]:https://en.wikipedia.org/wiki/CAN_bus
[CANopen]:https://en.wikipedia.org/wiki/CANopen`
[DBus]:https://en.wikipedia.org/wiki/D-Bus
[Sitara AM335x]:https://www.ti.com/processors/sitara-arm/am335x-cortex-a8/overview.html
