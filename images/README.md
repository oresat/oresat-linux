# Building OreSat Linux images
Uses beagleboard's [image-builder](https://github.com/beagleboard/image-builder)

## How to use
- **Note**: requires an internet connection (it has to call `apt` a lot).
- **Note**: on a pocketbeagle or a beagleboard black this will take ~40 minutes
- `./build_images.sh <board>` where `<board>` can be:
    - `cfc` - The image for oresat's cfc board.
    - `generic` - A generic image for custom oresat boards.
    - `gps` - The image for oresat GPS board.
    - `live` - The image for the oresat live board.
    - `startracker` - The image for oresat's startracker board.

## Notes on images generated
- Debian with the 4.19 Linux kernel
- All images have the user oresat with password temppwd
- The ip address is 192.168.6.2 (uses systemd-networkd for networking)
- CAN1 bus is enabled (uses systemd-networkd for networking)
- Root login over ssh is disabled
- CPU freqency is set to 300Mhz on boot
- Both eMMC0 and CAN1 device tree overlays are enabled
- Apt has both suggested and recommended autoinstall turned off
- **On the 1st boot**, it will grow the partition to fill the eMMC (or SD card) and reboot automatically

## Making a new board config
- `cp configs/oresat-generic.conf configs/<board-name>.conf` and change as needed.
- If a custom `uEnv.txt` is needed for device tree overlays make a `configs/<board-name>-uEnv.txt` and update the `chroot_post_uenv_txt` line in `configs/<board-name.conf`. **Note**: the custom contents of the new `<board>-uEnv.txt` will be append to the default `uEnv.txt` when the image is built.
- If a custom chroot script is needed `cp configs/oresat-generic.sh configs/<board-name>.sh`, change `configs/<board-name>.sh` as needed, and update `chroot_scripts` line in `configs/<board-name>.conf`.
