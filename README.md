# OreSat-Linux
This repo has all the general design and code for all 4 linux boards on OreSat. The four boards being GPS, StarTracker, OreSat Live, and Cirrus Flux Camera (CFC).

## CANdaemon
The inferface between the CAN2 bus and the main process on the linux board. DBus is used for IPC (inter-process comuincation) on the linux boards between the CANdaemon and the main process.

## UBoot
TBD

## Updater
Scripts to handle an update on the linux boards.

