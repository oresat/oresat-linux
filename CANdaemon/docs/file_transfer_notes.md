# File Transfer over CAN
For the linux cards on OreSat there are two object dictionary entry to help with file transfer. The 0x3001 record deals with write files to the CANdaemon and the 0x3002 record deals with read files from the CANdaemon.

## Write a file (OD entry 0x3001)
### Subindexes for record
- 0: Default. Size of record.
- 1: File name as a DOMAIN type
- 2: File data as a DOMAIN type
- 3: Files available as a 8 bit unsigned interger
- 4: Save file. Represented as a 8 bit unsigned interger

### How file receiving works:
The master node can write data to both file name and file data indexes and then write any 8 bit number to the Save file index to save the file into CANdaemon's receive directory. 


## Read a file (OD entry 0x3002)
### Subindexes for record
- 0: Default. Size of record.
- 1: File name as a DOMAIN type.
- 2: File data as a DOMAIN type.
- 3: Files available as a 8 bit unsigned interger.
- 4: Load file. Represented as a 8 bit unsigned interger.

### How file sending works:
Anything that wants a file sent to the master node would call APP_ODF_3002() and pass in the file path as the argument. This will copy the file into CANdaemon's send directory and increase the Files available count (subindex 3). The master node for the CANbus can read the File available subindex and if it want a file send to it, it would read the Load file subindex; this will load a files data into the file name and file data buffers. Then the master node can read both file name and file data indexes and complete the sending of the file.
