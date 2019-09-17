# BeagleBone Notes

## Routing Internet to Beaglebone from a linux host laptop
- On the Beaglebone terminal
    - `sudo route add default gw 192.168.7.1`
    - `sudo echo "nameserver 8.8.8.8" >> /etc/resolv.conf`
- In the linux host computer terminal
    - replace <WIRLESS> with wireless info from ip link (something like wlp3s0)
    - replace <ETHERNET> with wireless info from ip link (something like enp0s26f7u1u3)
    - `sudo sysctl net.ipv4.ip_forward=1`
    - `iptables --table nat --append POSTROUTING --out-interface <WIRELESS> -j MASQUERADE`
    - `iptables --append FORWARD --in-interface <ETHERNET> -j ACCEPT`

## Enabling CANbus on the BeagleBone
- Edit /boot/uEnt.txt
    - add line `dtb-overlay=/lib/firmware/BB-CAN1-00A0.dtbo` under Custom Cape
    - comment out `enable_uboot_cape_universal=1`
    - reboot BeagleBone
- Turn on CAN0
    - `sudo ip link set can0 up type can bitrate 1000000`

## DBus dependencies on BeagleBone
### For sd-bus
- gcc, make, libsystemd-dev, pkg-source
### For pydbus
- python3, gbus, python-pydbus, gbus-gLib, glib2, glibc

