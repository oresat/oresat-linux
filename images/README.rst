Building OreSat Linux images
============================

Uses beagleboard's https://github.com/beagleboard/image-builder

- **Note**: requires an internet connection (it has to call `apt` a lot).
- **Note**: on a pocketbeagle or a beagleboard black this will take >40 minutes

**Install dependencies**::

    $ sudo apt install git device-tree-complier zstd

**Build image**::
    
    $ ./build_images.sh <board>
  
where <board> can be
  - **cfc** - The image for OreSat's CFC (Cirrus Flux Camera) board.
  - **dev** - A basic image for development. All other images are derived from this image.
  - **dxwifi** - The image for the OreSat DxWiFi board.
  - **generic** - A generic image for custom OreSat boards.
  - **gps** - The image the OreSat's GPS board.
  - **star-tracker** - The image for the OreSat's Star Tracker board.
