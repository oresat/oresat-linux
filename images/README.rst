Building OreSat Linux images
============================

Uses beagleboard's https://github.com/beagleboard/image-builder

Requirements
------------

- Image must be built on a armv7 device running Debian.
- An internet connection (it has to call `apt` a lot).

Set up
------

- install dependencies

  .. code-block::

    $ sudo apt install git zstd device-tree-compiler make cpp m4 gcc dosfstools kpartx wget parted

- clone the oresat-linux repo

  .. code-block::

    $ git clone https://github.com/oresat/oresat-linux

Build Image
-----------

.. note:: On a PocketBeagle or a BeagleBoard Black this will take ~40 minutes.
   On a Raspberry Pi 3B it takes ~20 minutes.

.. code-block::

    $ cd oresat-linux/images/
    $ ./build_images.sh <board>
  
where <board> can be
    - **cfc** - The image for OreSat's CFC (Cirrus Flux Camera) board.
    - **dev** - A basic image for development. All other images are derived
      from this image.
    - **dxwifi** - The image for the OreSat DxWiFi board.
    - **generic** - A generic image for custom OreSat boards.
    - **gps** - The image the OreSat's GPS board.
    - **star-tracker** - The image for the OreSat's Star Tracker board.
