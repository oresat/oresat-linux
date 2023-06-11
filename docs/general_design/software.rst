Software
========

.. note:: The OLM (OreSat Linux Manager), multiple daemons, and D-Bus design
   was deprecated in favor of OLAF (OreSat Linux App Framework).

U-Boot
------

The first piece of software booted up on the AM335x is U-Boot.

U-Boot will read the AM335x EEPROM to figure out what specific device it is
and will load the device tree assoicated with the value in the EEPROM and then
boot Linux and pass off the device tree to the Linux kernel.

See `U-Boot Wikipedia`_ more info.

Device Tree
-----------

Device trees are used to describe the hardware, so hardware info does need to
be hardcoded into the Linux kernel; this include configuing pin modes. For
more info about device tree and Linux, see
https://www.kernel.org/doc/html/latest/devicetree/usage-model.html
Device tree overlays are used by the Linux kernel to overlay changes to the
Linux kernel's live Device Tree.

.. note:: For now, OreSat cards use the PocketBeagle's device tree and use overlays
    to apply card specific changes to pins.

Most the AM335x pins have 8 modes they can be configured between.
See https://www.ti.com/lit/ds/symlink/am3358.pdf for all the modes.

TI's SYSCONFIG can be used to make device trees for the AM335x. It can be
found at https://www.ti.com/tool/SYSCONFIG

For device tree overlay, a bunch of device tree overlay for the AM335x can
be found at https://github.com/beagleboard/bb.org-overlays/tree/master/src/arm
and are useful to use as examples.

Linux Enviroment
----------------

All Linux cards will use the 5.10 TI Debian kernel as it has support for PRUs
on the `Octavo A8`_ from `TI`_.

OreSat images are build with BeagleBoard's image-builder and make use of
BeagleBoard's scripts, services, and configurations.

Only `dpkg`_  will be used for all non-Python package with the ``python3``,
``python3-all``, ``python3-setuptools``, and ``python3-pip`` packages as the
execptions. All other Python packages will be installed with `pip`_.

All networking on OreSat cards is handled by systemd-networkd.

Kernel Modules
--------------

All OreSat Linux kernel module use `DKMS`_ as it will make installing and
packaging kernel module easier. Also, it's nice to no worry about updating
kernel modules if the kernel needs to be updated.

All kernel module will be put in a Debian package, so it can easily be
updated with the the updater build into `OLAF`_ and used to build to flight
OreSat linux images.

OLAF app
--------

.. note::  This is just a high level overview for OLAF. For specifics, see
   `OLAF Read the Docs`_ .

OreSat Linux App Framework (aka OLAF) is a Pythonic application framework for all
OreSat Linux cards. It is built ontop on `python-canopen`_. It is designed to
handle all the common OreSat CANopen Node functionality including support for ECSS
CANBus Extended Protocol, file transfer over CAN, and updating the Linux card thru
Python packages, Debian packages, and bash scripts.

Each oresat card will have its own OLAF-based app that will be daemonized that will
be the main process for the card. This OLAF app will put health and status info out
on the CAN bus, as well as let the C3 command it over the CAN bus.

Non-OLAF apps
-------------

Non-OLAF apps can be used, but if possible it should be avoided, it's hard enough to
mantain all the OreSat software projects.

Any non-OLAF app should have interfaces (like sockets), that OLAF app can connect to
get health / status info as well as control the non-OLAF app.

All Python projects will package and hosted on `PyPI`_ and all non-Python projects
will have Debian packages, so they can be updated thru OLAF and be used to build
flight OreSat Linux images.


Boot Process
------------

- The AM335x's ``LCD_DATA2`` pin, aka Octavo's ``G1`` pin, is the boot select pin.
  On power the AM335x will check so see if the this input. If set high
  (3.3v) the AM335x will try to boot off the eMMC, if set low the AM335x
  will try to boot off the SD card.
- The AM335x will now boot `U-Boot`_ image of the select storage device.
- `U-Boot`_ will read the AM335x's EEPROM to figure out what specific AM335x
  device it is. OreSat cards will have the Beagleboard's PocketBeagle value
  wriiten to its EEPROM, so `U-Boot`_ will then load the PocketBeagle's device
  tree.
- `U-Boot`_ will then try to boot the Linux kernel and will pass of the device
  tree to the Linux kernel. Every step below is has time based of when the
  kernel starts.
- The Linux kernel will apply any device tree overlays.
- Systemd starts up.
- DKMS loads any external kernel modules.
- Systemd starts the card's OLAF app.
- CAN bus is relase by the kernel. OLAF configures the CAN bus and starts sending
  out CANopen heartbeats.
- PRUs are available (if configured in device tree).

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
.. _BeagleBoard: https://beagleboard.org/
.. _U-Boot Wikipedia: https://en.wikipedia.org/wiki/Das_U-Boot
.. _U-Boot: https://en.wikipedia.org/wiki/Das_U-Boot
