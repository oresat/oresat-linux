# To Enable CAN on the Beaglebone

## Edit /boot/uEnt.txt
- add line `dtb-overlay=/lib/firmware/BB-CAN0-00A0.dtbo` under Custom Cape
- commit out `enable_uboot_cape_universal=1`

## Turn on CAN0
- `sudo ip link set can0 up type can bitrate 500000`

