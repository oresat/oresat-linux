# Building OreSat Linux images
- `./build_images.sh <board>` where <board> can be:
    - dev - A good image for bbb and pocket beagles.
    - generic - A generic image for custom oresat boards.
    - startracker - The image for startracker board.

# Notes
- Debian with the Linux kernel 4.19
- All images will use systemd-networkd for all networking (internet & CAN)

# Add more oresat board
- `cp configs/oresat-generic.conf configs/<board-name>.conf` and change as needed.
- If a costume `uEnv.txt` is needed for device tree overlays make a `configs/<board-name>-uEnv.txt` and update the `chroot_post_uenv_txt` line in `configs/<board-name.conf`. Note, the custom contents of uEnv.txt will be append to the default uEnv.txt when the image is built.
- If a costume chroot script is needed `cp configs/oresat-generic.sh configs/<board-name>.sh` and update `chroot_scripts` line in `configs/<board-name>.conf`.
