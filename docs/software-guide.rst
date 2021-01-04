============================
OreSat Software design guide
============================
All OreSat Linux software project should device into three catorgies; Linux Images (Debian/uboot), daemons, or kernel modules.

OS
==
- All board will use the 4.19 TI Debian kernel as it has support for PRUs and `beagleboard`_ has so much support for that kernel with Octavo A8.

Daemon
======

- There is no human user to start a process when the board is powered on, so
  all oresat programs shoule be daemonized (aka Linux Service) that can be
  control with systemd.
- The `oresat-linux-manager`_ is the only enabled OreSat daemon (starts on boot)
  as it is the front end API to the board. 
- Follow deamon service file naming of oresat-boardd.service, ie
  oresat-gpsd.service.

DBus
====

The `oresat-linux-manager`_ acts as the frontend and interface for the CANbus, therefor the main projects can be implemented in mostly any programing language and not have to worry about finding / making a CAN library following CANopen specifications for that specific language.

General
-------

- Only use system dbus not session dbus (aka user dbus).
- Follow dbus config file naming of org.oresat.<service>, ie org.oresat.startracker.

When to use
-----------
- Use methods to controlling and commanding daemons.
- Use signals for async reply to dbus methods. (The method sets a flag varible on the server and returns. Then the server process the flag, do whaterver is needed, and uses a dbus signal to responed.)
- Methods should be very quick function calls, `oresat-linux-manager`_ only has one thread per dbus interface.
- Use properties for the daemons public data; ie GPS would have a dbus property for the state vector of the satellite (postion, velocity, time stamp) and another property for its current state. 
- Use properties with the "emit when changed signal flag" set. This is for `oresat-linux-manager`_ to know when to read properties and not just spinning reading every x seconds.

Library recomendataions
-----------------------

- For C or C++ use `sd-bus`_ as it is the dbus library as it is built into systemd.
- For modern C++ (>= C++11) use `sdbus-cpp`_ as it is a nice modern C++ wrapper ontop of `sd-bus`_.
- For Python3 use `pydbus`_ as it is easy to use and mantained.
- For others? A list of supported dbus library for other langauges can be found at `freedesktop dbus bindings`_

Kernel Modules
==============

- Uses `DKMS`_, it will make installing and packaging kernel module easier. And its nice to no worry about updating kernel modules when kernel updates.

Debian package
==============

- All oresat software should be put in a Debian package, so it can easily be updated thru the `oresat-linux-updater`_. Even python projects. Don't want to have to deal with multiple package managers.
- Install system dbus conf files to /usr/share/dbus-1/system.d/.
- Install systemd service file to /lib/systemd/systemd/
- Only the `oresat-linux-manager`_ will be enable (starts on boot).
- No serives should be stated thru debian package bash files.
- If it is a python project make a setup.py to use it in debian/rules.

.. code-block:: bash

    #!/usr/bin/make -f

    %:
            dh $@ --with python3 --buildsystem=pybuild

- Projects that use compiled langauges should make binary debian package (precompile), less compliling on the satellite the better.

.. OreSat repos
.. _oresat-linux-manager: https://github.com/oresat/oresat-linux-manager
.. _oresat-linux-updater: https://github.com/oresat/oresat-linux-updater

.. Other repos
.. _CANopenNode: https://github.com/CANopenNode/CANopenNode
.. _sd-bus: https://github.com/systemd/systemd/blob/master/src/systemd/sd-bus
.. _sdbus-cpp: https://github.com/Kistler-Group/sdbus-cpp/
.. _pydbus: https://github.com/LEW21/pydbus

.. Other links
.. _CAN: https://en.wikipedia.org/wiki/CAN_bus
.. _CANopen: https://en.wikipedia.org/wiki/CANopen
.. _DBus: https://en.wikipedia.org/wiki/D-Bus
.. _DKMS: https://github.com/dell/dkms
.. _beagleboard: https://beagleboard.org/
.. _freedesktop dbus bindings: https://www.freedesktop.org/wiki/Software/DBusBindings/
