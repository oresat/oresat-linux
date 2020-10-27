# Building OreSat Linux images
- Debian with the Linux kernel 4.19
- All images will use systemd-networkd for all networking (internet & CAN)
- Uses beagleboard's [image-builder](https://github.com/beagleboard/image-builder)

## How to use
- **Note**: on a pocketbeagle or a beagleboard black this will take ~40 minutes.
- `./build_images.sh <board>` where `<board>` can be:
    - `generic` - A generic image for custom oresat boards.
    - `gps` - The image for oresat GPS board.
    - `startracker` - The image for oresat startracker board.

## Making a new board config
- `cp configs/oresat-generic.conf configs/<board-name>.conf` and change as needed.
- If a custom `uEnv.txt` is needed for device tree overlays make a `configs/<board-name>-uEnv.txt` and update the `chroot_post_uenv_txt` line in `configs/<board-name.conf`. **Note**: the custom contents of the new `<board>-uEnv.txt` will be append to the default `uEnv.txt` when the image is built.
- If a custom chroot script is needed `cp configs/oresat-generic.sh configs/<board-name>.sh`, change `configs/<board-name>.sh` as needed, and update `chroot_scripts` line in `configs/<board-name>.conf`.
