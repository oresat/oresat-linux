# Eample Code
The example code for DBus uses GDBus for C and pydbus for Python.


## DBus Server
 - Defines the DBus interface with an XML
 - Outputs two different signals (file signal and data signal). 
 - Process any method calls.

## DBus Signal Client
 - Print any signals found on the DBus interface.

## DBus Method Client
 - Call the Command and quit methods.


## Depenedices
### For the C version
- gbus, gbus-gLib, glib2, glibc, make
### For the Python version
- gbus, python-pydbus, gbus-gLib, glib2, glibc


## Usefull Links
 -  [DBus-Specs](https://dbus.freedesktop.org/doc/dbus-specification.html)
 -  [GNOME-GIO-Test-Code](https://gitlab.gnome.org/GNOME/glib/tree/master/gio/tests)
 -  [GDBus-Codegen](https://developer.gnome.org/gio/stable/gdbus-codegen.html)
 -  [Pydbus-Docs](https://pydbus.readthedocs.io/en/latest/)
 -  [Pydbus-GitHub](https://github.com/LEW21/pydbus)
