# OreSat-Linux
This repo has all the general design and code for all 4 linux boards on OreSat. The four boards being [GPS], [StarTracker], [OreSatLive], and Cirrus Flux Camera (CFC).

All A8 onboard OreSat will run 3 process; the CANdaemon, the updater, and the main process.

![](docs_and_examples%20OreSatLinuxDiagram.jpg)

## CANdaemon
The inferface between the CANbus and the any other processes on a linux board; including the main process and the Updater process. Uses DBus for IPC (inter-process communication). This based off of CANopenSocket but with multiple DBus interfaces.

## Updater
Python scripts to handle updating packages, source code, and drivers on the linux boards. Will use DBus to commicate with CANdaemon.

## Main process
All main process have there own repos: [GPS], [StarTracker], [OreSatLive], and CFC.

<!-- Other oresat repos -->
[GPS]:https://github.com/oresat/oresat-gps-software
[StarTracker]:https://github.com/oresat/oresat-star-tracker
[OreSatLive]:https://github.com/oresat/oresat-dxwifi-software
