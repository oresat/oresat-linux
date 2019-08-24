# CANdaemon for OreSat
The CANdaemon is a CAN interface for the main process for the GPS, Star Tracker, OreSat Live, and the Cirrus Flux Camera linux systems. It is based on [CANopenSocket](https://github.com/CANopenNode/CANopenSocket), but without the network managment, with the local domain socket replaced with a dbus interface, and turned into a daemon. CANopenSocket follows the CiA (CAN-in-Automation) specs for CANopen. This CANdaemon will also follow the ECSS (European Cooperation for Space Standardization) CANBus Extended Protocal on top of CiA specs.


## To Enable CAN on the Beaglebone
### Edit /boot/uEnt.txt
 - add line `dtb-overlay=/lib/firmware/BB-CAN0-00A0.dtbo` under Custom Cape
 - add line `dtb-overlay=/lib/firmware/BB-CAN1-00A0.dtbo` under Custom Cape
 - comment out `enable_uboot_cape_universal=1`
### Turn on CAN0
 - `sudo ip link set can0 up type can bitrate 1000000`
 - `sudo ifconfig can0 up`


## Usefull Links
 -  [CANopenSocket](https://github.com/CANopenNode/CANopenSocket)
 -  [DBus-Specs](https://dbus.freedesktop.org/doc/dbus-specification.html)
 -  [CANopen-Specs](https://www.can-cia.org/groups/specifications/)
 -  [ECSS-CANBus-Extended-Protocal](https://ecss.nl/standard/ecss-e-st-50-15c-space-engineering-canbus-extension-protocol-1-may-2015/)
