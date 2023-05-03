Building OreSat Linux images
============================

Uses BeagleBoard's https://github.com/beagleboard/image-builder

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
