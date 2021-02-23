OreSat Linux Updater
--------------------

.. note::

   This is just a high level overview for OreSat Linux Updater. For specifics see 
   https://oresat-linux.readthedocs.io/projects/oresat-linux-updater/en/latest/index.html

Basicily it's a simple daemon wrapper for on top of dpkg that allows the Linux
board to be updated over dbus using local Debian packages. It also support 
running bash scipts, but all updates should be done with deb packages. 

OreSat Linux boards are updated with Update Archive Files. They will contain 
all deb files and bash scripts and a instruction.txt file. THe instruction.txt
will define the order deb packages are installed in and the bash scripts are 
ran in.

If an update fails in any way, an  error signal will be sent to OLM and all
updates in the cache will be  deleted, as it is assume newer update required
the older updates to be successfully ran first.

The OreSat Linux Updater can also provides a tar with a txt file with a list of
files in the Update Archives cache and a copy of the dpkg status file. Both the
Update Archive file list and dpkg status file will be used to make future
Update Archives. 

.. _oresat-linux-manager: https://github.com/oresat/oresat-linux-manager
.. _oresat-linux-updater: https://github.com/oresat/oresat-linux-updater
