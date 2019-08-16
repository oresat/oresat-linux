# CANdaemon for OreSat

The CANdaemon is a CAN interface for the main process for the GPS, Star Tracker, OreSat Live, and the Cirrus Flux Camera linux systems. It is based on [CANopenSocket](https://github.com/CANopenNode/CANopenSocket), but without the network managment, with the local domain socket replaced with a dbus interface, and turned into a daemon.

# To Enable CAN on the Beaglebone
## Edit /boot/uEnt.txt
- add line `dtb-overlay=/lib/firmware/BB-CAN0-00A0.dtbo` under Custom Cape
- comment out `enable_uboot_cape_universal=1`
## Turn on CAN0
- `sudo ip link set can0 up type can bitrate 500000`

