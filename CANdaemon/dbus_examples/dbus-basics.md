# DBus basic
DBus is used for inter-process communication.

## Signals
Signal are messages that contain data that any process on that DBus interface can read. The server will emit the data and the client will subscribe and decode the data sent.

## Methods
Methods are functions that can be call by other processes. Method is implamented in the main process for the CANdaemon to call to issue commands to the mian process.
