General Board Software Design
=============================

All Linux board on OreSat will atleast have 3 services (aka daemons); OLM,
OreSat Linux Updater, and a main service. 

OreSat CANbus
-------------

The communication bus on OreSat is a CAN bus. All board on OreSat follow the
CANopen spec and use the `CANopenNode`_ library. **Note** All OreSat board
follow the `CCSDS Time Code Format`_ (CCSDS 301.0-B-4), not the CANopen
specification.

CANopen specifications can be found at https://can-cia.org/groups/specifications/

OLM (OreSat Linux Manager)
--------------------------

.. note::

   This is just a high level overview for OLM. For specifics, clone the
   `oresat-linux-manager`_ repo and build the Doxygen documentaton for it.

The manager acts a front end for all of OreSat Linux daemons and is build on
top of `CANopenNode`_.  It allows the `C3`_, Oresat's CAN Network Manger, to
control or get data from daemons on the Linux board. It uses `DBus`_ for
inter-process communication with daemons.  Basically the manager acts as a
CANopen interface to all services needed to control the board over `DBus`_.
In other words, its a common service to convert CANopen message to DBus
messsage and visa versa.

.. image:: oresat-linux-manager.jpg


OreSat Service Control
^^^^^^^^^^^^^^^^^^^^^^

OLM connects to systemd DBus and provides a API to the Network Manager on the
CAN bus.So in other words, OLM provides an API for `C3`_ to turn any OreSat
service on or off. It will on provide this option to only OreSat services
that are register, all non OreSat services can not be control, and all OreSat
services not registered will never be used. Supports up to 126 OreSat services.

File Transfer
^^^^^^^^^^^^^

OreSat Linux Manager supports both reading and writing files to the linux board
over CAN. Filename must be properly format otherwise OLM will delete them. The
formatting should make all file read to and written from Linux board to have
unique filenames.

**Format**::

   board_keyword_date.fileformat

**Format Rules:**

- all lowercase
- **board**: The board name / acronym; ie gps, st (aka star tracker), live, or cfc.
  This will allow the `C3`_ to know which board to write the file to.
- **keyword**: A **unique** key for the service for OLM to use when the file is
  being deliver to it; ie If a file has the "update" keyword, OLM will move it
  to location  defined by the OreSat Linux Updater app.
- **date**: The date the archive file was made. Must be in the YY-MM-DD-HH-mm-ss
  format, all sub fields are always 2 digit intergers.

    - YY: last two digits of the year
    - MM: month
    - DD: day
    - HH: hour (24-hour clock)
    - mm: minute
    - ss: second

- **fileformat**: Can be anything.
- '_' is only used to seperate the **board**, **keyword**, and **date** fields and
  nowhere else.

**Example, a update to the GPS board**::

   gps_update_21-10-03-14-30-27.tar.gz

Power Saving
^^^^^^^^^^^^

To save on power, when only OLM is running, it will set the CPU frequency to
powersave mode (300Mhz on AM335x), and when atleast one OreSat dameon is running
it will change the CPU frequency to performance (1GHz on AM335X).

Other features
^^^^^^^^^^^^^^

- Provides the interface for getting data to and from OreSat services over DBus.
- Provides info about the Linux Board; ie disk usage, RAM usage, etc.
- Interfaces with logind to provided power controls (poweroff and reboot).
- Interfaces with datetimed allow the board time to be changed
  (useful for CANopen SYNC messages)
- Follows the `CCSDS Time Code Format`_ (CCSDS 301.0-B-4) standards.

OreSat Linux Updater
--------------------

.. note::

   This is just a high level overview for OLM. For specifics, clone the
   `oresat-linux-updater`_ repo and build the sphinx documentaton for it.

A simple daemon wrapper for apt that allows the Linux board to be updated over
dbus using debian packages. It also support running bash scipts, but all 
updates should be done with deb packages. When started, it will not start 
updating until commanded to.

If an update fails in any way, an  error signal will be sent to OLM and all
updates in the cache will be  deleted, as it is assume newer update required
the older updates to be run first.

The OreSat Linux Updater can also provides a txt file with the list of all
install deb pacakges and their version simular to the output of::

    apt list --installed

Update Archive Files
^^^^^^^^^^^^^^^^^^^^

Update archive files should be a .tar.gz archive file and follow OLM's file
transfer filename format with the keyword "update". The date field in the
filename will be used to determine the next file to use as the oldest file is
always used.

**Example, a update to the GPS board**::

   gps_update_21-10-03-14-30-27.tar.gz

An update archive should contant deb files, bash scripts, files used by 
bash script, and **always** include a instructions.txt file. If there is no
instructions.txt in the archive file, the and all following update will be 
deleted and the error message will be sent to OLM.

**Example contents of a update archive**::

    instructions.txt
    package1.deb
    package2.deb
    package3.deb
    bash_script1.sh
    bash_script2.sh
    bash_script3.sh
    bash_script2_external_file

instructions.txt
^^^^^^^^^^^^^^^^

instruction.txt contatins a JSON string with with a list of instruction 
dictionaries with type and item fields. The instructions will be run in order.

The instruction types can be

- **install_pkg**: Install a deb package from the archive file.
- **remove_pkg**: Remove a deb package.
- **bash_script**: Run a bash script from the archive file.

**Example instructions.txt**::

    [
        {
            "type": "install_pkg",
            "item": "package1.deb"
        },
        {
            "type": "bash_script",
            "item": "bash_script1.sh"
        },
        {
            "type": "install_pkg",
            "item": "package2.deb"
        },
        {
            "type": "install_pkg",
            "item": "package3.deb"
        },
        {
            "type": "bash_script",
            "item": "bash_script2.sh"
        },
        {
            "type": "remove_pkg",
            "item": "package4"
        },
        {
            "type": "bash_script",
            "item": "bash_script3.sh"
        }
    ]

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
.. _CCSDS Time Code Format: https://public.ccsds.org/Pubs/301x0b4e1.pdf
