# CANdaemon CAN interface for OreSat Linux Updater interface
This is the CAN interface documentation for the OreSat Linux Updater. The Dbus documentation for the OreSat Linux Updater is under the main updater [directory](https://github.com/oresat/oresat-linux/blob/master/updater/oresat_side).

## Write a file (OD entry 0x3004)
| Sub Index | Usage                             | Data Type     | Access    |
| :-------: | :-------------------------------: | :-----------: | :-------: |
|     0     | Number of subindex in record      | uint8         | readonly  |
|     1     | Update Status                     | uint8         | readonly  |
|     2     | Current File                      | DOMAIN        | readonly  |
|     3     | Updates Available                 | uin32         | readonly  |
|     4     | Add Update File                   | DOMAIN        | writeonly |
|     5     | Update Now                        | boolena       | writeonly |
|     6     | Stop Update                       | boolean       | writeonly |
|     7     | Delete CurrentFile                | boolean       | writeonly |

### How updates work:
- The master CAN node can write in the paths to an update tar file for the updater to run in subindex 4. The updater will not run those updates until a boolean is written into subindex 5; at which point the updater will start or continue an update. If the master node need updater to stop, it can write a boolean to subindex 6.
- Subindex 1 is the current/last update status (negative for error, 0 for sleeping, 1 for updating, 2 for stopped).
- Subindex 2 and 3 is just useful information.
- Subindex 7 is for delete the update current update file in subindex 2 an any temp files make from uncompressed.
