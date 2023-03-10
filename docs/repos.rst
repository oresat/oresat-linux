Git Repositories
================

System Level Software
---------------------

Common software that is on or used by all OreSat Linux Boards.

- `oresat-linux`_ Image builder, the source for this documentation, and
  general utilities.
- `oresat-olaf`_ The OreSat Linux App Framework. Framework for all OreSat
  Linux applications.

C3 Board
---------

Command, Control, Commication board. The main flight board.

- `oresat-c3`_ Hardware design for the C3 board.
- `oresat-c3-software`_ The C3 application.
- ``oresat-ax5043-software`` The AX5043 application.
- `oresat-adcs-software`_ The ADCS library that controls the ADCS subsystem.

DxWiFi Board
------------

A board with a camera and intenna for sending live video of Earth over long 
distance WiFi. This is apart of the OreSat Live payload mission.

- `oresat-dxwifi-hardware`_ Hardware design for the OreSat Live board.
- `oresat-dxwifi-software`_  The DxWiFi application.
- `oresat-libdxwifi`_  Library for OreSat's 2.4GHz WiFi transmission system.

CFC (Cirrus Flux Camera) Board
------------------------------

Short wave infrared camera to map cirrus clouds. Another payload for OreSat.

- `oresat-cfc-software`_ The CFC application.
- `oresat-prucam-pirt1280`_ A kernel module for interfacing to PIRT1280 camera with PRUs.

GPS Board
---------

SDR / Hardware GPS receiver to calculate the location of satellite.

- `oresat-gps-hardware`_ Hardware design for the GPS board.
- `oresat-gps-software`_ The GPS application.

Star Tracker Board
------------------

Camera system that takes images of stars and uses the pattern of the stars let
to figure out which way the satellite is pointing.

- `oresat-star-tracker`_ Hardware design for the Star Tracker board.
- `oresat-star-tracker-software`_ The Star Tracker application.
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
.. _oresat-gps-software: https://github.com/oresat/oresat-gps-software
.. _oresat-gps-hardware: https://github.com/oresat/oresat-gps-hardware
.. _oresat-star-tracker: https://github.com/oresat/oresat-star-tracker
.. _oresat-star-tracker-software: https://github.com/oresat/oresat-star-tracker-software
.. _oresat-prucam-ar013x: https://github.com/oresat/oresat-prucam-ar013x
.. _oresat-dxwifi-hardware: https://github.com/oresat/oresat-dxwifi-hardware
.. _oresat-dxwifi-software: https://github.com/oresat/oresat-dxwifi-software
.. _oresat-libdxwifi: https://github.com/oresat/oresat-libdxwifi
.. _oresat-cfc-software: https://github.com/oresat/oresat-cfc-software
.. _oresat-prucam-pirt1280: https://github.com/oresat/oresat-prucam-pirt1280
.. _CANopen-Monitor: https://github.com/oresat/CANopen-Monitor
.. _oresat-tpane: https://github.com/oresat/oresat-tpane
.. _eds-utils: https://github.com/oresat/eds-utils
