Linux Images
============

All OreSat Linux Repos are hosted at https://images.oresat.org/

Notes about all images
----------------------

- All img are compress with zstd. To decompress them::

    $ zstd -d oresat-*.img.zst

- All images use Debian with the TI 5.10 kernel.
- All images have the user oresat with password temppwd.
- Root login over ssh is disabled.
- The CPU freqency governor is set to powersave (300Mhz on AM335x) on boot
- Both eMMC0 and CAN1 device tree overlays are enabled.
- Apt has both suggested and recommended autoinstall turned off.
- All use `systemd-networkd`_ for networking.
- Has two staic IP address just like BeagleBoard's images; `192.168.6.2` (for
  \*nix based) or `192.168.7.2` (for Windows).
- Images will try to route internet from hosts an IP addresses of `192.168.6.1`
  and  `192.168.7.1`.
- The CAN1 bus is enabled.
- The hostname is set to the name of board it's for; ie generic, star-tracker,
  etc.
- All image are minimize to reduce the size of the `.img`, so the image will
  need to be expanded on first boot.

Images
------

**oresat-dev-*.img**

The dev image is the image all other images are derived from. It has no 
OreSat packages installed, not even OLM, so it is a great image to use for
development. This image should work fine on all OreSat Boards, a 
`BeagleBone Black`_, or a `PocketBeagle`_.

**oresat-generic-*.img**

This a basicly the dev image with only OLM and the oresat linux updater
installed.

**oresat-<board>-*.img**

All specific board images are the dev image plus all software and system 
configurations required for that board. 

.. warning:: Board specific images expect the hardware for their specific
   board, so the the software on these image may not work on other hardware.

Common changes to do on first boot
----------------------------------

- To expanded the image to use the entire SD card / eMMC run::
 
    $ sudo /opt/scripts/tools/grow_partition.sh
    $ sudo reboot

- Stop / disable OLM::

    $ sudo systemctl stop oresat-linux-managerd
    $ sudo systemctl disable oresat-linux-managerd

- It may be nice to always have the CPU freqency at 1GHz. Make sure to stop
  and disable OLM first, if it is installed. To set the cpufreq to 1GHz on boot
  edit `/etc/default/cpufrequtils` to be `GOVERNOR="performance"` and/or set it
  to performance mode now::

    $ sudo cpufreq-set -g performance

.. _BeagleBone Black: https://beagleboard.org/black/
.. _PocketBeagle: https://beagleboard.org/pocket
.. _systemd-networkd: https://wiki.archlinux.org/index.php/Systemd-networkd
