Debian
======

All Linux board will use the 4.19 TI Debian kernel as it has support for PRUs 
and `beagleboard`_ has so much support for that kernel with Octavo A8.

Only apt will be used as a pacakge manager. The goal is to avoid using other 
pacakge managers like pip as much as possible. Don't want to have to deal with
multiple package managers.

All networking will be handled by systemd-networkd. 

OreSat Linux images are host at https://debian.oresat.org/images/. 

.. _beagleboard: https://beagleboard.org/
