# Example Code
The example code for dbus; sd-bus is for c and pydbus is for python3.


### DBus Server
 - To use the system dbus add org.example.project.oresat.conf to /etc/dbus-1/sytem.d/.
 - Defines the DBus interface with an XML
 - Outputs two different signals (file signal and data signal). 
 - Process any method calls.

### DBus Signal Client
 - Prints any signals found on the DBus interface.

### DBus Method Client
 - Calls the Command and Quit methods.

### DBus Property Client
 - Calls the read and print all properties.


## Depenedices for beaglebone black
### For the sd-bus version
- gcc, make, libsystemd-dev, pkg-source
### For the Python version
- python3, gbus, python-pydbus, gbus-gLib, glib2, glibc
