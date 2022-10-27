Software
========

.. note::

    The OLM (OreSat Linux Manager), multiple daemons, and dbus design was
    deprecated in favor of OLAF (OreSat Linux App Framework).

Linux Enviroment
----------------

All Linux boards will use the 5.10 TI Debian kernel as it has support for PRUs
on the `Octavo A8`_ from `TI`_. As well as make use of several projects
provided by `beagleboard`_.

Only `dpkg`_  will be used for all non-python package with the ``python3``,
``python3-all``, ``python3-setuptools``, and ``python3-pip`` packages as the
execptions. All other python packages will be installed with `pip`_.

All networking will be handled by systemd-networkd. 


OLAF
----

.. note::

   This is just a high level overview for OLAF. For specifics, see
   `OLAF Read the Docs`_ .

OreSat Linux App Framework (aka OLAF) is a pythonic application framework for all
OreSat Linux boards. It is built ontop on `python-canopen`_. It is designed to
handle all the common OreSat CANopen Node functionality including support for ECSS
CANBus Extended Protocol, file transfer over CAN, and updating the Linux board.

Each oresat board will have its own OLAF app that will be daemonized. All python 
projects will package and hosted on `PyPI`_.


Kernel Modules
--------------

All OreSat Linux kernel module use `DKMS`_ as it will make installing and
packaging kernel module easier. Also, it's nice to no worry about updating
kernel modules if the kernel needs to be updated.

All kernel module will be put in a Debian package, so it can easily be
updated with the the updater build into `OLAF`_. 


.. OreSat repos
.. _OLAF: https://github.com/oresat/oresat-olaf

.. OreSat Read the Docs
.. _OLAF Read the Docs: https://oresat-olaf.readthedocs.io/en/latest/

.. Other repos
.. _DKMS: https://github.com/dell/dkms
.. _python-canopen: https://github.com/christiansandberg/canopen

.. Other links
.. _dpkg: https://www.dpkg.org/
.. _pip: https://pypi.org/project/pip/
.. _PyPI: https://pypi.org
.. _TI: https://www.ti.com/processors/sitara-arm/am335x-cortex-a8/overview.html
.. _Octavo A8: https://octavosystems.com/octavo_products/osd335x-sm/
.. _beagleboard: https://beagleboard.org/
