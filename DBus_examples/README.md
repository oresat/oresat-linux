# DBus example
It used for inter process communication on Linux (provided by systemd). 
Freedesktop explains it best at: [dbus freedesktop](https://www.freedesktop.org/wiki/Software/dbus/).

## DBus Quick Guide
### Terminology
- **Methods** are functions that can be call by other processes over DBus.
- **Signals** are messages that contain data that any process on that DBus interface can read.
- **Properties** are data that can be seen by any other process on that DBus interface.

### System vs User Bus
- There is only one system bus, while every user has a user (also called session) bus.
- To use the system dbus add a dbus config file to `/etc/dbus-1/sytem.d/` or `/usr/share/dbus-1/system.d/`.
- This file will define who can access that system dbus interface. 
- See [org.OreSat.Example.conf](org.OreSat.Example.conf) as an example.

### busctl
- busctl is a way to monitor dbus interfaces in a terminal
- To list all system dbus interfaces: `busctl`
- To show all methods, signals, and properties for that dbus interface: `busctl introspect <DBUS-INTERFACE> <DBUS-PATH>`


## Example dependencies
- For debian
    - For the sd-bus version: `apt install gcc make libsystemd-dev pkg-source`
    - For the Python version: `apt install python3 python-pydbus`
- For arch
    - For the sd-bus version: `pacman -S gcc make pkg-source`
    - For the Python version: `pacman -S python3 python-pydbus`

## How to run
- `sudo cp org.OreSat..conf /usr/share/dbus-1/system.d/`
- sd bus examples
    - `make clean all`
    - run an example (need two terminals)
        - sudo ./sd-bus-property-example server
        - sudo ./sd-bus-property-example client
- pybus examples
    - run an example (need two terminals)
        - sudo python3 spybus-property-example server
        - sudo python3 pybus-property-example client

## Usefull Links
 -  [DBus-Specs](https://dbus.freedesktop.org/doc/dbus-specification.html)
 -  [sd-bus-header](https://github.com/systemd/systemd/blob/master/src/systemd/sd-bus.h)
 -  [sd-bus-example](http://0pointer.net/blog/the-new-sd-bus-api-of-systemd.html)
 -  [Pydbus-GitHub](https://github.com/LEW21/pydbus)
 -  [Pydbus-Docs](https://pydbus.readthedocs.io/en/latest/)
 -  [GNOME-GIO-Test-Code](https://gitlab.gnome.org/GNOME/glib/tree/master/gio/tests)
 -  [GDBus-Codegen](https://developer.gnome.org/gio/stable/gdbus-codegen.html)

