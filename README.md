# OreSat-Linux
This repo has all the general design and code for all 4 linux boards on OreSat. The four boards being GPS, StarTracker, OreSat Live, and Cirrus Flux Camera (CFC).

## CANdaemon
The inferface between the CANbus and the any other processes on a linux board; including the main process and the Updater process. Uses DBus for IPC (inter-process communication). This based off of CANopenSocket but with multiple DBus interfaces.

## UBoot
TBD

## Updater
Python scripts to handle updating packages, source code, and drivers on the linux boards. Will use DBus to commicate with CANdaemon.

