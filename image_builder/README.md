# Building OreSat Linux images

Uses BeagleBoard's https://github.com/beagleboard/image-builder

## Files

- `chroot_scripts/early-oresat-chroot.sh`: The early chroot script, used to do anything before
  the image config ares parsed in chroot environment.
- `chroot_scripts/oresat-chroot.sh`: The main chroot script that runs after most packages are
  installed in chroot environment.
- `scripts/`: Helpful scripts that will be placed in `/opt/scripts/` in built images.
- `configs/*.conf`: All the image configs; defines what packages are install, system settings,
  etc.

## How it Works

**NOTE:** Most of the build process is handled by BeagleBoard's image-builder, with some custom
OreSat scripts executed at specific moments.

- U-Boot bootloader is compiled.
- All device tree overlays are compiled.
- A temp directory is made, the directory is mounted with chroot.
- OreSat's early chroot script is executed (all device tree overlay binaries are copied over).
- A bunch of BeagleBoard's scripts are executed to build the image; including installing all Debian
  and Python packages. Basically most things defined in the image `.conf` file happen during
  this step.
- OreSat's custom chroot script is executed.
- Chroot environment is exited.
- A partition is made on the system.
- All files from the temp directory are copied over to the new partition.
- U-Boot image is added to partition.
- A `.img` file is made from the partition.
- The `.img` file is compressed with zst.
- A SHA-256 is generated for the `.img.zst`.

## Requirements

- Image must be built on a device running Debian or a Debian-based distro.
- An internet connection (it has to call `apt` a lot).
- U-boot requires cross compilation if outside of ARMv7 environment (i.e. arm-none-eabi-gcc). 

## Set up

Install dependencies

```bash
$ sudo apt install git zstd device-tree-compiler make cpp m4 gcc dosfstools kpartx wget parted tree bc
```

Clone the oresat-linux repo

```bash
$ git clone https://github.com/oresat/oresat-linux
```

## Build Image

```bash
$ cd oresat-linux/image_builder/
$ ./build_images.sh <board>
```

Where `<board>` can be:

- `c3`: The flight image for OreSat's C3 board.
- `cfc`: The flight image for OreSat's CFC (Cirrus Flux Camera) board.
- `dev`: A general image for development.
- `dxwifi`: The flight image for the OreSat DxWiFi board.
- `generic`: A generic minimal flight-like image for any OreSat board.
- `gps`: The flight image the OreSat's GPS board.
- `star-tracker`: The flight image for the OreSat's Star Tracker board.

# U-Boot

## Setup

Install the GCC cross compiler for ARM Cortex-R/M processors

- Arch Linux
  ```bash
  sudo pacman -S arm-none-eabi-gcc openssl bc
  ```
- Debian Linux
  ```bash
  sudo apt install gcc-arm-none-eabi libssl-dev bc
  ```

## Build

This will generate `u-boot.img` and `MLO` in the oresat_bootloader folder.

```bash
$ make
```

## Flash

Flash `u-boot.img` and `MLO`. Change `/dev/sda` as needed. You can also use  
scripts/write_uboot.sh.
  
```bash
$ sudo dd if=u-boot-dtb.img of=/dev/sda count=4 seek=1 bs=384k
$ sudo dd if=MLO of=/dev/sda count=2 seek=1 bs=128k
```
## U-Boot console
  
You can connect to the u-boot console using serial. On a beaglebone
black this is done by connecting a FTDI cable between your computer's 
USB port and the beaglebone black. You can then use a utility like
picocom to connect. For example:
```
picocom -b 115200 /dev/serial/by-id/usb-FTDI_FT232R_USB_UART_AC01OGK9-if00-port0 
```
Press the boot button and power on the beaglebone black. U-boot will start 
producing output to serial. Pressing the spacebar, when propted, drops you 
into the uboot console. If boot_delay=0 you will have to hold down the spacebar 
or it will skip the console. 

The device must be set to i2c0 to interact with the EEPROM using i2c.   
To set the device, copy this command into the u-boot prompt (i.e. =>).   
```
i2c dev 0 
```
Reads 0x20 bytes from the EEPROM at address 0x50 offset 0x00 
```
i2c md 0x50 0x00.2 20 
```
Writes 1 byte (42) to the EEPROM at address 0x50 offset 0x08 
```
i2c mw 0x50 0x08.2 42  
```
### Example EEPROM writes to produce meaningful boot strings  
  
#### BNLT00C0 
  
```
i2c mw 0x50 0x08.2 42 
```
```
i2c mw 0x50 0x09.2 4e  
```
```
i2c mw 0x50 0x0a.2 4c  
```
```
i2c mw 0x50 0x0b.2 54  
```
```
i2c mw 0x50 0x0c.2 30  
```
```
i2c mw 0x50 0x0d.2 30  
```
```
i2c mw 0x50 0x0e.2 43  
```
```
i2c mw 0x50 0x0f.2 30  
```
  
#### OSC30100  
  
```
i2c mw 0x50 0x08.2 4f 
```
```
i2c mw 0x50 0x09.2 53 
```
```
i2c mw 0x50 0x0a.2 43 
```
```
i2c mw 0x50 0x0b.2 33 
```
```
i2c mw 0x50 0x0c.2 30 
```
```
i2c mw 0x50 0x0d.2 31 
```
```
i2c mw 0x50 0x0e.2 30 
```
```
i2c mw 0x50 0x0f.2 30 
```
