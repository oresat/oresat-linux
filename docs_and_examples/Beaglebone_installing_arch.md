# Installing Arch Linux on Beaglebone 

## Making u-boot files for beaglebone on host machine running Arch
- Get arm compiler
    - `pacman -S arm-none-eabi-gdb arm-none-eabi-gcc arm-none-eabi-binutils`
- Get uboot
    - `git clone https://github.com/u-boot/u-boot.git`
- Make MLO and u-boot.img
    - `cd u-boot`
    - `make am335x_evm_defconfig`
    - `ARCH=arm CROSS_COMPILE=arm-none-eabi- make`

## SD card with Arch linux:
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
    - `dd if=MLO of=/dev/sdX count=1 seek=1 conv=notrunc bs=128k`
    - `dd if=u-boot.img of=/dev/sdX count=2 seek=1 conv=notrunc bs=384k`
    - `cp MLO mnt/boot/MLO`
    - `cp u-boot.img mnt/boot/u-boot.img`
    - `umount mnt`
    - `sync`

## Powering on Beaglebone Black
- On a Linux laptop run `ls /dev`
- Using a serial to usb cable connect a Beaglebone Black to latop running Linux.
- On the Linux laptop run `ls /dev` again, you should see a new entry like ttyUSB0
- connect power usb to Beaglebone black
- Run `screen ttyUSB0 115200` on the laptop to connected to the Beaglebone.
= Use the user: alarm and password: alarm
- Swap to root `su -` password: root. Sudo is not installed, so alarm user can't do much.

## Fixing hwclock
- Run `pwck` short for passwd check. Will return nothing if it passes.
- Run `systemctl --failed` to see if shadow.service has failed
- If both pwck and shadow.service are working skip this section. 
    - Mostly likely they failed to the hardware clock being in the past with passwd set times in the future. 
    - Run `hwclock --set --date='DD/MM/YYYY HH:MM:SS'` to set the hardware clock (use **UTC** time). 
    - Run `hwclock -s` to override the system time with the hardware time.

## Connecting Beaglebone to the internet
- plug in the Beagle into a router with a ethernet cable
- `systemctl enable dhcpcd` to enable dhcp client daemon on startup
- `systemctl start dhcpcd` to start dhcp client daemon
- `ping www.google.com` to test connection. May need to do a reboot.

## Update system
- Intialize pacman keyring
    - `pacman-key --init`
    - `pacman-key --populate archlinuxarm`
- `pacman -Syu` to update system
- `pacman -S make gcc python3 git pkg-config dtc` Install usefull packages as needed.



