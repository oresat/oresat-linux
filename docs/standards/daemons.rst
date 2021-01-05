Daemons
=======

There is no human user to start a process when the board is powered on, so all
oresat programs shoule be daemonized (aka Linux Service) that can be control 
with systemd.

The `oresat-linux-manager`_ is the only enabled OreSat daemon (starts on boot)
as it is the front end API to the board. 

Follow deamon service file naming of oresat-<name>d.service, ie 
oresat-gpsd.service.

All oresat daemon should use DBus to interface with the 
`oresat-linux-manager`_. Since `oresat-linux-manager`_ is the front end to 
the Linux board and handels all the CANbus commication, so other oresat daemon
don't need to handle the following the CANopen sepcifications. Also, DBus has 
bindings in alot of langauges.

OreSat daemons should only use system dbus not session dbus (aka user dbus).
Follow dbus config file naming of org.oresat.<service>, ie 
org.oresat.startracker.

**DBus Library recomendataions**

- For C or C++ use `sd-bus`_ as it is the dbus library as it is built into 
  systemd.
- For modern C++ (>= C++11) use `sdbus-cpp`_ as it is a nice modern C++ wrapper
  ontop of `sd-bus`_.
- For Python3 use `pydbus`_ as it is easy to use and mantained.
- For others? A list of supported dbus library for other langauges can be found
  at `freedesktop dbus bindings`_

.. OreSat repos
.. _oresat-linux-manager: https://github.com/oresat/oresat-linux-manager
.. _oresat-linux-updater: https://github.com/oresat/oresat-linux-updater

.. Other repos
.. _CANopenNode: https://github.com/CANopenNode/CANopenNode
.. _sd-bus: https://github.com/systemd/systemd/blob/master/src/systemd/sd-bus
.. _sdbus-cpp: https://github.com/Kistler-Group/sdbus-cpp/
.. _pydbus: https://github.com/LEW21/pydbus

.. Other links
.. _DBus: https://en.wikipedia.org/wiki/D-Bus
.. _freedesktop dbus bindings: https://www.freedesktop.org/wiki/Software/DBusBindings/
