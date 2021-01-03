General Board Software Design
=============================

All AM335x onboard OreSat will atleast have 3 services (aka daemons); the manager, the Linux updater, and the main service. 
OreSat uses `CAN`_ for commucation between systems onboard and follows the `CANopen`_ specifications.

OreSat Linux Manager
--------------------

The manager acts a front end for all of OreSat Linux daemons and is build on top of `CANopenNode`_. 
It allows the `C3`_, Oresat's CAN Network Manger, to control or get data from daemons on the Linux board. 
It uses `DBus`_ for inter-process communication with daemons. 
Basically the manager acts as a CANopen interface to all services needed to control the board over `DBus`_.
In other words, its a common service to convert CANopen message to DBus messsage and visa versa.

.. image:: oresat-linux-manager.jpg

- systemd interface will allow `C3`_ board to turn on/off any service on the board.
- logind to provided power controls.
- datetimed allow the board time to be changed (useful for CANopen SYNC messages)
- any custom oresat daemon for the `C3`_ to control and get data from it.


See the `oresat-linux-manager`_ repo for more info.

OreSat Linux Updater
--------------------

A simple daemon wrapper for apt that allows the Linux board to be updated over dbus using debian packages.
See the `oresat-linux-updater`_ repo for more info.

.. OreSat repos
.. _C3: https://github.com/oresat/oresat-c3
.. _oresat-linux-manager: https://github.com/oresat/oresat-linux-manager
.. _oresat-linux-updater: https://github.com/oresat/oresat-linux-updater

.. Other repos
.. _CANopenNode: https://github.com/CANopenNode/CANopenNode

.. Other links
.. _CAN: https://en.wikipedia.org/wiki/CAN_bus
.. _CANopen: https://en.wikipedia.org/wiki/CANopen
.. _DBus: https://en.wikipedia.org/wiki/D-Bus
