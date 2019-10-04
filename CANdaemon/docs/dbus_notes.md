# DBus Notes

## Terminology
 - **Methods** are functions that can be call by other processes over DBus.
 - **Signal** are messages that contain data that any process on that DBus interface can read.
 - **Properties** are data that can be seen by any other process on that DBus interface.

## System vs User Bus
 - There is only one system bus, while every user has a user (also called session) bus.
 - To use the system dbus add a dbus config file to /etc/dbus-1/sytem.d/. This file will define who can access that dbus interface. [example conf] (./CANdaemon/dbus_examples/org.example.project.oresat.conf)

## busctl
 - busctl is a way to monitor dbus interfaces in a terminal
 - To list all system dbus interfaces `busctl`
 - To list all session (user) dbus interfaces `busctl --user`
 - To show all methods, signals, and properties for that dbus interface. `busctl introspect <DBUS-INTERFACE> <DBUS-PATH>`

## Enabling CANbus on the BeagleBone
- Edit /boot/uEnt.txt
    - add line `dtb-overlay=/lib/firmware/BB-CAN1-00A0.dtbo` under Custom Cape
    - comment out `enable_uboot_cape_universal=1`
    - reboot BeagleBone
- Turn on CAN0
    - `sudo ip link set can0 up type can bitrate 1000000`

## DBus dependencies on BeagleBone
### For sd-bus
- gcc, make, libsystemd-dev, pkg-source
### For pydbus
- python3, gbus, python-pydbus, gbus-gLib, glib2, glibc

## Usefull Links
 -  [DBus-Specs](https://dbus.freedesktop.org/doc/dbus-specification.html)
 -  [sd-bus-header](https://github.com/systemd/systemd/blob/master/src/systemd/sd-bus.h)
 -  [sd-bus-example](http://0pointer.net/blog/the-new-sd-bus-api-of-systemd.html)
 -  [Pydbus-GitHub](https://github.com/LEW21/pydbus)
 -  [Pydbus-Docs](https://pydbus.readthedocs.io/en/latest/)
 -  [GNOME-GIO-Test-Code](https://gitlab.gnome.org/GNOME/glib/tree/master/gio/tests)
 -  [GDBus-Codegen](https://developer.gnome.org/gio/stable/gdbus-codegen.html)
