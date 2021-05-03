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

    $ sudo apt install git device-tree-complier zstd

- clone the oresat-linux repo

  .. code-block::

    $ git clone https://github.com/oresat/oresat-linux

Build Image
-----------

.. note:: On a pocketbeagle or a beagleboard black this will take >40 minutes

.. code-block::

    $ cd oresat-linux/images/
    $ ./build_images.sh <board>
  
where <board> can be
    - **cfc** - The image for OreSat's CFC (Cirrus Flux Camera) board.
    - **dev** - A basic image for development. All other images are derived from this image.
    - **dxwifi** - The image for the OreSat DxWiFi board.
    - **generic** - A generic image for custom OreSat boards.
    - **gps** - The image the OreSat's GPS board.
    - **star-tracker** - The image for the OreSat's Star Tracker board.
