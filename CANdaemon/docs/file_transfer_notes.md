# File Transfer over CAN
For the linux cards on OreSat there are three object dictionary entries to help with file transfer. The 0x3001 record only deals with writing files to the CANdaemon. Both of the 0x3002 and 0x3003 records deal with reading files from the CANdaemon. 0x3002 is a array of files that could be read and 0x3003 is used to read a file and control the array of files.

## Write a file (OD entry 0x3001)
| Sub Index | Usage                          | Data Type   | Access    |
| :-------- | :----------------------------: | :---------: | :-------: |
|     0     | number of subindex in record   | uint8       | readonly  |
|     1     | file name                      | DOMAIN      | writeonly |
|     2     | file data                      | DOMAIN      | writeonly |
|     3     | save file                      | DOMAIN      | writeonly |

### How file writing works:
The master node can write data to both file name and file data indexes and then write any 8 bit number to the Save file index to save the file into CANdaemon's receive directory. 

## Read a file (OD entry 0x3002 and 0x3003)
### 0x3002
| Sub Index | Usage                          | Data Type   | Access   |
| :-------- | :----------------------------: | :---------: | :------: |
|     0     | number of subindex in record   | uint8       | readonly |
|     1     | file name 1                    | DOMAIN      | readonly |
|    ...    | ...                            | DOMAIN      | readonly |
|    127    | file name 127                  | DOMAIN      | readonly |

### 0x3003
| Sub Index | Usage                              | Data Type   | Access    |
| :-------- | :--------------------------------: | :---------: | :-------: |
|     0     | Number of subindex in record       | uint8       | readonly  |
|     1     | Current index in file name array   | uint8       | readwrite |
|     2     | File path                          | DOMAIN      | readonly  |
|     3     | File data                          | DOMAIN      | readonly  |
|     4     | File size                          | uint32      | readonly  |
|     5     | Delete file in current index       | boolean     | readwrite |
|     6     | Total file available               | uint32      | readonly  |
|     7     | Number of files not in file array  | uint32      | readonly  |
|     8     | Refresh the file name array        | boolean     | readwrite |

### How file reading works:
- The master node can read the file array (0x3002) and pick which sub-index in the array it wants to read. It can choose the file by writing the sub-index value of 0x3002 into 0x3003 sub-index 1. That will load the file info into sub-indexes 2, 3, and 4 (file path, file data, and file size buffers respectively). Then the master node can read any of those file buffers as it wants to. 
- The file can be deleted by reading or write a value to sub-index 5. 
- Sub-index 6 and 7 are just extra useful information for the master node (number of files that could be read and number of file that are not loaded into the file array). 
- Sub-index 8 allows the master node to refresh the file name array (OD entry 0x3002). This is useful to add any files that could not be added to the file name array, if the array was full at on point.
