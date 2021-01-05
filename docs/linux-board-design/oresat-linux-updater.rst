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
