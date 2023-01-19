Git Repositories
================

System Level Software
---------------------

Common software that is on or used by all OreSat Linux Boards.

- `oresat-linux`_ Image builder, the source for this documentation, and
  general utilities.
- `oresat-olaf`_ The OreSat Linux App Framework. Framework for all OreSat
  Linux applications.

GPS Board
---------

SDR/Hardware GPS receiver to calculate the location of satellite. Also will
include software for off loading the controls for ACS subsystem.

- `oresat-gps-hardware`_ Hardware design for the GPS board.
- `oresat-gps-software`_ GPS application.
- `oresat-adcs-software`_ The ADCS Manager library that controls the ADCS
  subsystem.

Star Tracker Board
------------------

Camera system that takes images of stars and uses the pattern of the stars let
to figure out which way the satellite is pointing.

- `oresat-star-tracker`_ Hardware design for the Star Tracker board.
- `oresat-star-tracker-software`_ The Star Tracker application.
- `oresat-prucam-ar013x`_ A kernel module for interfacing to AR013x camera with PRUs.

DxWiFi Board
------------

A board with a camera and intenna for sending live video of Earth over long 
distance WiFi. This is apart of the OreSat Live payload mission.

- `oresat-dxwifi-hardware`_ Hardware design for the OreSat Live board.
- `oresat-dxwifi-software`_  Software for OreSat's 2.4GHz WiFi transmission
  system
- `oresat-live-software`_ Software for both the satellite and ground components
  of OreSat Live. 

CFC (Cirrus Flux Camera) Board
------------------------------

Short wave infrared camera to map cirrus clouds. Another payload for OreSat.

- `oresat-cfc-software`_ CFC application.
- `oresat-prucam-pirt1280`_ A kernel module for interfacing to PIRT1280 camera with PRUs.

Support Software
----------------

Support software projects for other OreSat projects.

- `CANopen-Monitor`_ An NCurses-based TUI application for tracking activity
  over a CAN bus and decoding messages.
- `oresat-tpane`_ A NCurses library for easier development of TUI applications.
- `eds-utils`_ Utilities to edit, view, and convert CANopen EDS and DCF files.

.. OreSat repos
.. _oresat-linux: https://github.com/oresat/oresat-linux
.. _oresat-linux-manager: https://github.com/oresat/oresat-linux-manager
.. _oresat-linux-updater: https://github.com/oresat/oresat-linux-updater
.. _oresat-adcs-software: https://github.com/oresat/oresat-adcs-software
.. _oresat-gps-software: https://github.com/oresat/oresat-gps-software
.. _oresat-gps-hardware: https://github.com/oresat/oresat-gps-hardware
.. _oresat-star-tracker: https://github.com/oresat/oresat-star-tracker
.. _oresat-star-tracker-software: https://github.com/oresat/oresat-star-tracker-software
.. _oresat-prucam-ar013x: https://github.com/oresat/oresat-prucam-ar013x
.. _oresat-dxwifi-hardware: https://github.com/oresat/oresat-dxwifi-hardware
.. _oresat-dxwifi-software: https://github.com/oresat/oresat-dxwifi-software
.. _oresat-live-software: https://github.com/oresat/oresat-live-software
.. _oresat-cfc-software: https://github.com/oresat/oresat-cfc-software
.. _oresat-prucam-pirt1280: https://github.com/oresat/oresat-prucam-pirt1280
.. _CANopen-Monitor: https://github.com/oresat/CANopen-Monitor
.. _oresat-tpane: https://github.com/oresat/oresat-tpane
.. _eds-utils: https://github.com/oresat/eds-utils
