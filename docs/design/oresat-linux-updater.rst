OreSat Linux Updater
--------------------

.. note::

   This is just a high level overview for OLM. For specifics, clone the
   `oresat-linux-updater`_ repo and build the sphinx documentaton for it.

Basicily it's a simple daemon wrapper for on top of apt (Debian package 
manager) that allows the Linux board to be updated over dbus using local Debian
packages. It also support running bash scipts, but all updates should be done
with deb packages. 

OreSat Linux boards are updated with Update Archive Files. They will contain 
all deb files and bash scripts.

.. toctree::
    :maxdepth: 2

    ../standards/update-archive-files

If an update fails in any way, an  error signal will be sent to OLM and all
updates in the cache will be  deleted, as it is assume newer update required
the older updates to be run first.

The OreSat Linux Updater can also provides a txt file with the list of all
install deb pacakges and their version simular to the output of::

    apt list --installed

.. _oresat-linux-manager: https://github.com/oresat/oresat-linux-manager
.. _oresat-linux-updater: https://github.com/oresat/oresat-linux-updater
