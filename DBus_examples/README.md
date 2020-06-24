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
    - For the sd-bus version: `gcc make libsystemd-dev pkg-config`
    - For the Python version: `python3 libsystemd-dev python3-pydbus`
- For arch
    - For the sd-bus version: `gcc make systemd-lib pkg-config`
    - For the Python version: `python3 systemd-lib python-pydbus`

## How to
- `sudo cp org.OreSat.Example.conf /usr/share/dbus-1/system.d/`
- Run make on sd-bus examples
    - `make clean all`
- Run an example (need two terminals)
    - sudo python3 pybus-property-server-example
    - sudo ./sd-bus-property-client-example

## Usefull Links
 -  [DBus-Specs](https://dbus.freedesktop.org/doc/dbus-specification.html)
 -  [sd-bus-header](https://github.com/systemd/systemd/blob/master/src/systemd/sd-bus.h)
 -  [sd-bus-example](http://0pointer.net/blog/the-new-sd-bus-api-of-systemd.html)
 -  [Pydbus-GitHub](https://github.com/LEW21/pydbus)
 -  [Pydbus-Docs](https://pydbus.readthedocs.io/en/latest/)
 -  [GNOME-GIO-Test-Code](https://gitlab.gnome.org/GNOME/glib/tree/master/gio/tests)
 -  [GDBus-Codegen](https://developer.gnome.org/gio/stable/gdbus-codegen.html)


