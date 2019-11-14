# Controller Area Network (CAN) Notes

## Beaglebone CAN Notes

### Enabling CANbus on the BeagleBone Black
- Edit /boot/uEnt.txt
    - add line `dtb-overlay=/lib/firmware/BB-CAN1-00A0.dtbo` under Custom Cape
    - comment out `enable_uboot_cape_universal=1`
    - reboot BeagleBone
- Turn on CAN0
    - `sudo ip link set can0 up type can bitrate 1000000`

### Enabling CANbus on the pocketbeagle
- `sudo config-pin P2_09 can`
- `sudo config-pin P2_11 can`
- `sudo ip link set can1 up type can bitrate 1000000`
- `sudo ifconfig can1 up`
- `sudo ifconfig can1 txqueuelen 10000`


## [CANable](https://canable.io/)

### enable the CANable using slcand firmware
-`sudo slcand -o -c -f -s8 <PATH_TO_CANABLE> can0`
-`sudo ip link set can0 up`

### enable the CANable using candlelight firmware
-`sudo ip link set can0 up type can bitrate 1000000`


## Virtual CAN

### enable vcan
-`sudo ip link add dev vcan0 type vcan`
-`sudo ip link set vcan0 up`

### disable vcan
-`sudo ip link set vcan0 down`
-`sudo ip link del dev vcan0`
