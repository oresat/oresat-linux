Git Repositories
================

System Level Software
---------------------

Common software that is on or used by all OreSat Linux Boards.

- `oresat-linux`_ Image builder, the source for this documentation, and
  general utilities.
- `oresat-linux-manager`_ The front end daemon for all OreSat Linux boards.
- `oresat-linux-updater`_ A daemon that allows all boards to be updated.

GPS Board
---------

SDR/Hardware GPS receiver to calculate the location of satellite. Also will
include software for off loading the controls for ACS subsystem.

- `oresat-gps-hardware`_ Hardware design for the GPS board.
- `oresat-gps-software`_ GPS daemon.
- `oresat-adcs-software`_ The ADCS Manager daemon that controls the ADCS
  subsystem.

Star Tracker Board
------------------

Camera system that takes images of stars and uses the pattern of the stars let
to figure out which way the satellite is pointing.

- `oresat-star-tracker`_ Hardware design for the Star Tracker board.
- `oresat-star-tracker-software`_ The Star Tracker daemon.
- `oresat-linux-prucam`_ A kernel module for interfacing to camera with a PRU.

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

- `oresat-cfc-software`_ CFC daemon.
- `oresat-linux-prucam`_ A kernel module for interfacing to camera with a PRU.

Support Software
----------------

Support software projects for other OreSat projects.

- `CANopen-Monitor`_ An NCurses-based TUI application for tracking activity
  over a CAN bus and decoding messages.
- `oresat-tpane`_ A NCurses library for easier development of TUI applications.

.. OreSat repos
.. _oresat-linux: https://github.com/oresat/oresat-linux
.. _oresat-linux-manager: https://github.com/oresat/oresat-linux-manager
.. _oresat-linux-updater: https://github.com/oresat/oresat-linux-updater
.. _oresat-adcs-software: https://github.com/oresat/oresat-adcs-software
.. _oresat-gps-software: https://github.com/oresat/oresat-gps-software
.. _oresat-gps-hardware: https://github.com/oresat/oresat-gps-hardware
.. _oresat-star-tracker: https://github.com/oresat/oresat-star-tracker
.. _oresat-star-tracker-software: https://github.com/oresat/oresat-star-tracker-software
.. _oresat-dxwifi-hardware: https://github.com/oresat/oresat-dxwifi-hardware
.. _oresat-dxwifi-software: https://github.com/oresat/oresat-dxwifi-software
.. _oresat-live-software: https://github.com/oresat/oresat-live-software
.. _oresat-cfc-hardware: https://github.com/oresat/oresat-cfc-hardware
.. _oresat-linux-prucam: https://github.com/oresat/oresat-linux-prucam
.. _CANopen-Monitor: https://github.com/oresat/CANopen-Monitor
.. _oresat-tpane: https://github.com/oresat/oresat-tpane
