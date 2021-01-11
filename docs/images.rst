OreSat Linux Images
===================

All OreSat Linux Repos are hosted at https://debian.oresat.org/images/

To decompress them::

    $ zstd -d oresat-*.img.zst

Notes about all images
----------------------

- All images use Debian with the TI 4.19 kernel.
- All images have the user oresat with password temppwd.
- Root login over ssh is disabled.
- The CPU freqency governor is set to powersave (300Mhz on AM335x) on boot
- Both eMMC0 and CAN1 device tree overlays are enabled.
- Apt has both suggested and recommended autoinstall turned off.
- **On the 1st boot**, it will grow the partition to fill the eMMC (or SD card)
  and reboot automatically. This may take several minutes.
- All use `systemd-networkd`_ for networking.
- The static IP address is 192.168.6.2 and will route internet from a host 
  with an IP address of 192.168.6.1.
- The CAN1 bus is enabled.
- The hostname is set to the name of board it's for; ie generic, startracker,
  etc.

Generic Image
-------------

The generic image is the image all other images are derived from. It has no 
OreSat packages installed, not even OLM, so it is a great image to use for
development. This image should work fine on all OreSat Boards, a 
`BeagleBoneBlack`_, or a `PocketBeagle`_.

If the generic image is used for development, it may be nice to always have 
the CPU freqency at 1GHz. To set the cpufreq to 1GHz on boot edit 
`/etc/default/cpufrequtils` to be `GOVERNOR="performance"` and/or 
set it to performance mode now::

    sudo cpufreq-set -g performance

Specific Board Images
---------------------

.. warning:: These images expect the hardware for their specific board, so the
   the software on these image may not work on other hardware.

All specific board images are the generic image plus all software and system 
configurations required for that board. 

Building Images
---------------

See https://github.com/oresat/oresat-linux/tree/master/images

.. _BeagleBoneBlack: https://beagleboard.org/black/
.. _PocketBeagle: https://beagleboard.org/pocket
.. _systemd-networkd: https://wiki.archlinux.org/index.php/Systemd-networkd
