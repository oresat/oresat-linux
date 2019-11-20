# Controller Area Network (CAN) Notes


## Enabling CAN1 on the BeagleBone Black (pins P9_19 P9_20)
- Edit /boot/uEnt.txt
    - add line `dtb-overlay=/lib/firmware/BB-CAN1-00A0.dtbo` under Custom Cape
    - comment out `enable_uboot_cape_universal=1`
    - reboot BeagleBone
- Turn on CAN0
    - `sudo ip link set can0 up type can bitrate 1000000`


## Enabling CAN1 on the pocketbeagle (pins P2_09 P2_11)
- Edit /boot/uEnt.txt
    - add line `dtb-overlay=/lib/firmware/PB-CAN1-00A0.dtbo` under Custom Cape
    - comment out `enable_uboot_cape_universal=1`
    - reboot PocktetBeagle
- Turn on CAN1
    - `sudo ip link set can1 up type can bitrate 1000000`


## [CANable](https://canable.io/)
- enable the CANable using slcand firmware
    - `sudo slcand -o -c -f -s8 <PATH_TO_CANABLE> can0`
    - `sudo ip link set can0 up`
- enable the CANable using candlelight firmware
    - `sudo ip link set can0 up type can bitrate 1000000`


## Virtual CAN
- enable vcan
    - `sudo ip link add dev vcan0 type vcan`
    - `sudo ip link set vcan0 up`
- disable vcan
    - `sudo ip link set vcan0 down`
    - `sudo ip link del dev vcan0`
