Git Repositories
================

System Level Software
---------------------

Common software that is on or used by all OreSat Linux Cards.

- `oresat-linux`_ Image builder, the source for this documentation, and
  general utilities.
- `oresat-olaf`_ The OreSat Linux App Framework. Framework for all OreSat
  Linux applications. Built ontop of Python's CANopen library.

C3 Card
---------

Command, Control, Commication card. The main flight card.

- `oresat-c3`_ Hardware design for the C3 card.
- `oresat-c3-software`_ The main C3 OLAF-based application.
- `oresat-ax5043-driver`_ The AX5043 radio driver.
- `oresat-adcs-software`_ The ADCS library that controls the ADCS subsystem.

DxWiFi Card
------------

A card with a camera and antenna for sending live video of Earth over long 
distance WiFi. This is a part of the OreSat Live payload mission.

- `oresat-dxwifi-hardware`_ Hardware design for the OreSat Live card.
- `oresat-dxwifi-software`_  The main DxWiFi OLAF-based application.
- `oresat-live-software`_  TBD
- `oresat-libdxwifi`_  Library for OreSat's 2.4GHz WiFi transmission system.

CFC (Cirrus Flux Camera) Card
------------------------------

Short wave infrared camera to map cirrus clouds. Another payload for OreSat.

- `oresat-cfc-software`_ The CFC OLAF-based application.
- `oresat-prucam-pirt1280`_ A kernel module for interfacing to PIRT1280 camera with PRUs.

GPS Card
---------

SDR / Hardware GPS receiver to calculate the location of satellite.

- `oresat-gps-hardware`_ Hardware design for the GPS card.
- `oresat-gps-software`_ The GPS OLAF-based application.

Star Tracker Card
------------------

Camera system that takes images of stars and uses the pattern of the stars let
to figure out which way the satellite is pointing.

- `oresat-star-tracker`_ Hardware design for the Star Tracker card.
- `oresat-star-tracker-software`_ The Star Tracker OLAF-based application.
- `oresat-prucam-ar013x`_ A kernel module for interfacing to AR013x camera with PRUs.

Support Software
----------------

Support software projects for other OreSat projects.

- `CANopen-Monitor`_ An NCurses-based TUI application for tracking activity
  over a CAN bus and decoding messages.
- `oresat-tpane`_ A NCurses library for easier development of TUI applications.
- `eds-utils`_ Utilities to edit, view, and convert CANopen EDS and DCF files.

.. OreSat repos
.. _oresat-linux: https://github.com/oresat/oresat-linux
.. _oresat-olaf: https://github.com/oresat/oresat-olaf
.. _oresat-adcs-software: https://github.com/oresat/oresat-adcs-software
.. _oresat-c3: https://github.com/oresat/oresat
.. _oresat-c3-software: https://github.com/oresat/oresat-c3-software
.. _oresat-ax5043-driver: https://github.com/oresat/oresat-ax5043-driver
.. _oresat-gps-software: https://github.com/oresat/oresat-gps-software
.. _oresat-gps-hardware: https://github.com/oresat/oresat-gps-hardware
.. _oresat-star-tracker: https://github.com/oresat/oresat-star-tracker
.. _oresat-star-tracker-software: https://github.com/oresat/oresat-star-tracker-software
.. _oresat-prucam-ar013x: https://github.com/oresat/oresat-prucam-ar013x
.. _oresat-dxwifi-hardware: https://github.com/oresat/oresat-dxwifi-hardware
.. _oresat-dxwifi-software: https://github.com/oresat/oresat-dxwifi-software
.. _oresat-live-software: https://github.com/oresat/oresat-live-software
.. _oresat-libdxwifi: https://github.com/oresat/oresat-libdxwifi
.. _oresat-cfc-software: https://github.com/oresat/oresat-cfc-software
.. _oresat-prucam-pirt1280: https://github.com/oresat/oresat-prucam-pirt1280
.. _CANopen-Monitor: https://github.com/oresat/CANopen-Monitor
.. _oresat-tpane: https://github.com/oresat/oresat-tpane
.. _eds-utils: https://github.com/oresat/eds-utils
