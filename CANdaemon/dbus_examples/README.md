# Eample Code
The example code for dbus. sd-bus and gdbus are in c. pydbus is in python3.

### Terminology
 - **Signal** are messages that contain data that any process on that DBus interface can read.
 - **Methods** are functions that can be call by other processes over DBus.

### System vs User Bus
 - There is only one system bus, while every user has a user (also called session) bus.
 - To use the system dbus add org.example.project.oresat.conf to /etc/dbus-1/sytem.d/.

### DBus Server
 - Defines the DBus interface with an XML
 - Outputs two different signals (file signal and data signal). 
 - Process any method calls.

### DBus Signal Client
 - Prints any signals found on the DBus interface.

### DBus Method Client
 - Calls the Command and Quit methods.


## Depenedices for beaglebone black
### For the sd-bus version
- gcc, make, libsystemd-dev, pkg-source
### For the gdbus version
- gcc, gbus, gbus-gLib, glib2, glibc, make, pkg-source
### For the Python version
- python3, gbus, python-pydbus, gbus-gLib, glib2, glibc


## Usefull Links
 -  [DBus-Specs](https://dbus.freedesktop.org/doc/dbus-specification.html)
 -  [GNOME-GIO-Test-Code](https://gitlab.gnome.org/GNOME/glib/tree/master/gio/tests)
 -  [GDBus-Codegen](https://developer.gnome.org/gio/stable/gdbus-codegen.html)
 -  [Pydbus-Docs](https://pydbus.readthedocs.io/en/latest/)
 -  [Pydbus-GitHub](https://github.com/LEW21/pydbus)
 -  [sd-bus-example](http://0pointer.net/blog/the-new-sd-bus-api-of-systemd.html)
