# Installing Arch Linux on Beaglebone 

## Making u-boot files for Beaglebone
Follow [Uboot Notes](uboot-resources.md) to make MLO and u-boot.img files.

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

Mostly likely the Beaglebone will have some hardware clock / passwd error. So a lot of stuff like ssh will not work.

## Fixing hwclock
- On a Linux laptop run `ls /dev/tty*`
- Using a serial to usb cable connect a Beaglebone Black to latop running Linux.
- On the Linux laptop run `ls /dev/tty*` again, you should see a new entry like ttyUSB0
- connect power usb to Beaglebone black
- Run `screen ttyUSB0 115200` on the laptop to connected to the Beaglebone.
- Use the user: alarm and password: alarm
- Swap to root `su -` password: root. Sudo is not installed, so alarm user can't do much.
- Run `pwck` short for passwd check. Will return nothing if it passes.
- Run `systemctl --failed` to see if shadow.service has failed
- If both pwck and shadow.service are working skip this section. 
    - `hwclock --set --date='DD/MM/YYYY HH:MM:SS'` to set the hardware clock (use **UTC** time). 
    - `hwclock -s` to override the system clock with the hardware clock.

## Connecting Beaglebone to the internet
- Plug in the Beaglebone into a router with a ethernet cable
- `systemctl enable dhcpcd` to enable dhcp client daemon on startup
- `systemctl start dhcpcd` to start dhcp client daemon
- `ping www.google.com` to test connection. May need to do a reboot.
- You should be able swap from serial to ssh now. Run `ip a` on beaglebone to ip address or get info from router.

## Update system
- Intialize pacman keyring
    - `pacman-key --init`
    - `pacman-key --populate archlinuxarm`
- `pacman -Syu` to update system
- `pacman -S make vim gcc python3 git pkg-config dtc` Install usefull packages as needed.

## Make alarm user a sudoer
- `pacman -S sudo`
- run `EDITOR=vim visudo` and uncomment out `%wheel ALL=(ALL) ALL`
- `usermod -G wheel alarm` Add alarm to wheel group.
- `groups alarm` Check if alarm is in the wheel group.

