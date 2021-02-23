OreSat Linux Project Standards
==============================

.. note:: All OreSat software projects should either be a daemon or a Linux
   kernel module.

Debian
------

All Linux boards will use the 4.19 TI Debian kernel as it has support for PRUs
on the `Octavo A8`_ from `TI`_ and `beagleboard`_.

Only `APT`_  will be used as a package manager. The goal is to avoid using other 
pacakge managers like `pip`_ as much as possible. Don't want to have to deal with
multiple package managers.

All networking will be handled by systemd-networkd. 

All OreSat Linux images are host at https://debian.oresat.org/images/. 

Debian Packages
---------------

All OreSat software should be put in a Debian package, so it can easily be
updated with the `oresat-linux-updater`_. Also, projects that use compiled 
langauges should make binary debian package (precompile) as the less compliling
on the satellite the better.

All OreSat Linux packages are upload to https://debian.oresat.org/packages/, so
they can be used to build OreSat Linux images. 

Daemons
-------

There is no human user to start a process when the board is powered on, so all
OreSat programs should be a daemon (aka Linux Service) so it can be control 
with systemd. The `oresat-linux-manager`_ will use systemd to start or stop 
OreSat daemons as command by the `C3`_ board.

The `oresat-linux-manager`_ is the only enabled (starts on boot) OreSat daemon
as it is the front end to the board. 

Follow deamon service file naming of oresat-<name>d.service, e.g.
oresat-gpsd.service.

Daemons should log to syslog (journald will grab those logs), so if the logs
are needed OLM can easily copy them and give them to the `C3`_.

All OreSat daemons will use D-Bus to interface with the `oresat-linux-manager`_.

OreSat daemons should use session dbus (aka user dbus) unless it is a root
process then use the system dbus. If the daemon uses system dbus follow dbus
config file naming format of org.oresat.<service>, e.g. org.oresat.startracker.

**D-Bus Library recomendataions**

- For C or C++ use `sd-bus`_ as it is the dbus library as it is built into 
  systemd.
- For modern C++ (>= C++11) use `sdbus-cpp`_ as it is a nice modern C++ wrapper
  ontop of `sd-bus`_.
- For Python3 use `pydbus`_ as it is easy to use and mantained.
- For others? A list of supported dbus library for other langauges can be found
  at `freedesktop dbus bindings`_

Kernel Modules
--------------

Uses `DKMS`_ as it will make installing and packaging kernel module easier. 
Also, it's nice to no worry about updating kernel modules if the kernel needs
to be updated.

.. OreSat repos
.. _C3: https://github.com/oresat/oresat-c3
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
.. _D-Bus: https://en.wikipedia.org/wiki/D-Bus
.. _freedesktop dbus bindings: https://www.freedesktop.org/wiki/Software/D-BusBindings/
.. _beagleboard: https://beagleboard.org/
