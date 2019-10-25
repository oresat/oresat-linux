# Beaglebone CAN Notes

## Enabling CANbus on the BeagleBone Black
- Edit /boot/uEnt.txt
    - add line `dtb-overlay=/lib/firmware/BB-CAN1-00A0.dtbo` under Custom Cape
    - comment out `enable_uboot_cape_universal=1`
    - reboot BeagleBone
- Turn on CAN0
    - `sudo ip link set can0 up type can bitrate 1000000`

## Enabling CANbus on the pocketbeagle
- `sudo config-pin P2_09 can`
- `sudo config-pin P2_11 can`
- `sudo ip link set can1 up type can bitrate 1000000`
- `sudo ifconfig can1 up`
- `sudo ifconfig can1 txqueuelen 10000`
