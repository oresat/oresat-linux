# OreSat Linux Updater (onboard)
This is daemon is available on on all OreSat Linux board and will allow any debian package to be to be updated/installed. It can be controlled through it dbus interface. Archive files can be send to the Linux Updater through the dbus interface. The archive file will contain *.deb packages and a yaml instruction file. Reading the instruction file it will update only packages for that specific Linux board on OreSat.

## Dependacies
- python3, python3-yaml, python3-pydbus, python3-apt, python3-daemon

### Dbus methods
| Name              | Usage                                                     | Inputs    | Output    |
| :---------------: | :-------------------------------------------------------: | :-------: | :-------: | 
| StartUpdated()    | Starts the update, if there is archive file available.    | -         | Boolean   |
| AddArchiveFile()  | Add new archive file to queue.                            | String    | Boolean   |

### Dbus properties
| Name                  | Usage                                             | Datatype  | Read/Write    |
| :-------------------: | :-----------------------------------------------: | :-------: | :-----------: |
| Status                | Current state of Linux Updater.                   | Int32     | Readonly      |
| ComputerName          | Computer name that Updater will use.              | String    | ReadWrite     |
| CurrentArchiveFile    | The name of current file being installed.         | String    | Readonly      |
| AvailableArchiveFiles | Number of archive file available.                 | Uint32    | Readonly      |

### States Machine
![]("https://github.com/oresat/oresat-linux/updater_onboard/UpdaterStateMachine.jpg")
- **Sleep** - Initial state that will be listening for commands.
- **Pre-Update** - If there are archive files available, the updater will open the oldest one, and load the yaml instrution file. Then parses the instructions and install deb packages it says to.
- **Failed** - Delete all archive files and if it was in the Update state, it will revert the update.


