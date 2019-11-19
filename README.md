# OreSat-Linux
This repo has all the general design and code for all 4 linux boards on OreSat. The four boards being [GPS], [StarTracker], [OreSat Live], and Cirrus Flux Camera (CFC).

All A8 onboard OreSat will run 3 processes; the CANdaemon, the updater, and the main process.

![](https://github.com/oresat/oresat-linux/blob/master/docs_and_examples/drawio/OreSatLinuxDiagram.jpg)

## Processes
### CANdaemon
The inferface between the CANbus and the any other processes on a linux board; including the main process and the Updater process. Uses DBus for IPC (inter-process communication). This based off of [CANopenSocket] but with multiple DBus interfaces.

### Updater
Python scripts to handle updating packages on the linux boards. Will use DBus to commicate with CANdaemon.

### Main process
All main process have there own repos: [GPS], [StarTracker], [OreSat Live], and CFC. Will use DBus to commicate with CANdaemon.

## Repo Layout 
- **CANdaemon** - Source code for CANdaemon
- **docs_and_examples** - Useful documentation and examples for general Linux stuff 
- **updater** - Source code for Linux updarer

<!-- Other oresat repos -->
[GPS]:https://github.com/oresat/oresat-gps-software
[StarTracker]:https://github.com/oresat/oresat-star-tracker
[OreSat Live]:https://github.com/oresat/oresat-dxwifi-software

<!-- Other repos -->
[CANopenSocket]:https://github.com/CANopenNode/CANopenSocket
