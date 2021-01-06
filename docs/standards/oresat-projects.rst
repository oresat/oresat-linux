OreSat Linux Project Standards
==============================

All OreSat Linux board will use Debian. All OreSat software projects should 
either be a Daemon or a Linux kernel module.

Debian
------

All Linux board will use the 4.19 TI Debian kernel as it has support for PRUs.
Both `TI`_ and `beagleboard`_ has so much support for that kernel with 
`Octavo A8`_.

Only `APT`_  will be used as a package manager. The goal is to avoid using other 
pacakge managers like `pip`_ as much as possible. Don't want to have to deal with
multiple package managers.

All networking will be handled by systemd-networkd. 

All OreSat Linux images are host at https://debian.oresat.org/images/. 

Debian Packages
---------------

All oresat software should be put in a Debian package, so it can easily be
updated with the `oresat-linux-updater`_. Projects that use compiled 
langauges should make binary debian package (precompile), less compliling 
on the satellite the better.

All OreSat Linux packages are upload to https://debian.oresat.org/packages/, so
they can be added to OreSat images. 

Daemons
-------

There is no human user to start a process when the board is powered on, so all
oresat programs shoule be daemonized (aka Linux Service) that can be control 
with systemd. The `oresat-linux-manager`_ will use systemd to start or stop 
OreSat daemons.

The `oresat-linux-manager`_ is the only enabled OreSat daemon (starts on boot)
as it is the front end API to the board. 

Follow deamon service file naming of oresat-<name>d.service, ie 
oresat-gpsd.service.

Daemons should log to syslog so jounrnald can log. OLM can copy so they can be
sent back to Earth for debugging.

All oresat daemons will use DBus to interface with the `oresat-linux-manager`_.
Since `oresat-linux-manager`_ is the front end to the Linux board and handels 
all the CANbus commication, so other oresat daemon don't need to handle the 
following the CANopen sepcifications. Also, DBus has bindings in alot of 
langauges.

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

Kernel Modules
--------------

Uses `DKMS`_, it will make installing and packaging kernel module easier. And 
its nice to no worry about updating kernel modules when kernel updates.

.. OreSat repos
.. _oresat-linux-manager: https://github.com/oresat/oresat-linux-manager
.. _oresat-linux-updater: https://github.com/oresat/oresat-linux-updater

.. Other repos
.. _CANopenNode: https://github.com/CANopenNode/CANopenNode
.. _sd-bus: https://github.com/systemd/systemd/blob/master/src/systemd/sd-bus
.. _sdbus-cpp: https://github.com/Kistler-Group/sdbus-cpp/
.. _pydbus: https://github.com/LEW21/pydbus
.. _DKMS: https://github.com/dell/dkms

.. Other links
.. _APT: https://en.wikipedia.org/wiki/APT_(software)
.. _pip: https://pypi.org/project/pip/
.. _TI: https://www.ti.com/processors/sitara-arm/am335x-cortex-a8/overview.html
.. _Octavo A8: https://octavosystems.com/octavo_products/osd335x-sm/
.. _DBus: https://en.wikipedia.org/wiki/D-Bus
.. _freedesktop dbus bindings: https://www.freedesktop.org/wiki/Software/DBusBindings/
.. _beagleboard: https://beagleboard.org/
