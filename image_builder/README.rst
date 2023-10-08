Building OreSat Linux images
============================

Uses BeagleBoard's https://github.com/beagleboard/image-builder

Files
-----

- ``chroot_scripts/early-oresat-chroot.sh``: The early chroot script, used to do anything before
  the image config ares parsed in chroot environment.
- ``chroot_scripts/oresat-chroot.sh``: The main chroot script that runs after most packages are
  installed in chroot environment. 
  is used to set custom things on OreSat images.
- ``configs/*.conf``: All the image configs; defines what packages are install, system settings,
  etc.
- ``device_tree/*.dts``: Holds the device tree for flight images. This device tree configures
  some pins to there nessary mode and then configures all other pins to be gpio floating inputs.
  **NOTE:** The ``oresat-dev`` image uses the original PocketBeagle device.
- ``device_tree_overlays/*.dts``: These are the device tree overlays for specific images, all are 
  compiled and are copied to ``/lib/firmware/`` during by the early chroot script.
- ``uEnv/*-uEnv.txt``: The specific ``uEnv.txt`` for the image, defines what device tree overlays
  are loaded on boot.

How it Works
------------

.. note:: Most of the build process is handled by BeagleBoard's image-builder, with some custom
   OreSat scripts excuted at specific moments.

- All device tree overlays are compiled.
- A temp directory is made, the directory is mounted with chroot.
- OreSat's early chroot script is excuted (all device tree overlay binaries are copied over).
- A bunch of BeagleBone's scripts are excuted to build the image; including installing all Debian
  and Python packages. Basically most things defined in the image ``.conf`` file happen during
  this step.
- OreSat's custom chroot script is excuted.
- Chroot environment is exited.
- A partition is made on the system.
- All files from the temp directory are copied over to the new partition.
- U-Boot image is added to partition.
- A ``.img`` file is made from the partition.
- The ``.img`` file is compressed with zst.
- A SHA-256 is generated for the ``.img.zst``.

Requirements
------------

- Image must be built on a armv7 device running Debian or a Debian-based distro.
- An internet connection (it has to call ``apt`` a lot).

Set up
------

- install dependencies

  .. code-block::

    $ sudo apt install git zstd device-tree-compiler make cpp m4 gcc dosfstools kpartx wget parted tree

- clone the oresat-linux repo

  .. code-block::

    $ git clone https://github.com/oresat/oresat-linux

Build Image
-----------

.. note:: On a PocketBeagle or a BeagleBone Black this will take ~40 minutes.
   On a Raspberry Pi 3B it takes ~20 minutes.

.. code-block::

    $ cd oresat-linux/image_builder/
    $ ./build_images.sh <board>
  
where <board> can be
    - **c3** - The image for OreSat's C3 board.
    - **cfc** - The image for OreSat's CFC (Cirrus Flux Camera) board.
    - **dev** - A general image for development.
    - **dxwifi** - The image for the OreSat DxWiFi board.
    - **gps** - The image the OreSat's GPS board.
    - **star-tracker** - The image for the OreSat's Star Tracker board.
