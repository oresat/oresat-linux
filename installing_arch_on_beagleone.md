# Installing Arch Linux on beaglebone 

## Making u-boot files for beaglebone on host machine running Arch
- Get arm compiler
    - `pacman -S arm-none-eabi-gdb arm-none-eabi-gcc arm-none-eabi-binutils`
- Get uboot
    - `git clone https://github.com/u-boot/u-boot.git`
- Make MLO and u-boot.img
    - `cd u-boot`
    - `make am335x_evm_defconfig`
    - `ARCH=arm CROSS_COMPILE=arm-none-eabi- make`

## SD card with arch linux:
Replace **sdX** with device name. The `lsblk` command can be used to find the device name. Run all command commands as root. Based off of [armlinuxarm guide](https://archlinuxarm.org/platforms/armv7/ti/beaglebone-black).

- Zero the SD card:
    - `dd if=/dev/zero of=/dev/sdX bs=1M count=8`
-Start fdisk to partition the SD card:
    - `fdisk /dev/sdX`
- At the fdisk prompt, delete old partitions and create a new one:
    - Type o. This will clear out any partitions on the drive.
    - Type n, then p for primary, 1 for the first partition on the drive, 2048 for the first sector, and then press ENTER to accept the default last sector.
    - Write the partition table and exit by typing w.
- Create the ext4 filesystem:
    - `mkfs.ext4 /dev/sdX1`
- Mount the filesystem:
    - `mkdir mnt`
    - `mount /dev/sdX1 mnt`
- Download and extract the root filesystem:
    - `wget http://os.archlinuxarm.org/os/ArchLinuxARM-am33x-latest.tar.gz`
    - `bsdtar -xpf ArchLinuxARM-am33x-latest.tar.gz -C mnt`
    - `sync`
- Install the U-Boot bootloader:
    - `dd if=mnt/boot/MLO of=/dev/sdX count=1 seek=1 conv=notrunc bs=128k`
    - `dd if=mnt/boot/u-boot.img of=/dev/sdX count=2 seek=1 conv=notrunc bs=384k`
    - `cp MLO mnt/boot/MLO`
    - `cp u-boot.img mnt/boot/u-boot.img`
    - `umount mnt`
    - `sync`
- 
