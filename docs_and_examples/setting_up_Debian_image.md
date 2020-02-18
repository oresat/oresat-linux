# Setting up a default OreSat Debian image

## uboot overlays
Change these lines in /boot/uEnv.txt
```
uboot_overlay_addr4=/lib/firmware/BB-BONE-eMMC1-01-00A0.dtbo
uboot_overlay_addr5=/lib/firmware/PB-CAN1-00A0.dtbo
#enable_uboot_cape_universal=1
```

## systemd-networkd
Read the Arch [systemd-networkd] wiki page. It is a great systemd-networkd guide.
### Add can config to systemd-networkd
Make the file `/etc/systemd/network/10-can.network`. Change Name as needed.
```
[Match]
Name=can1

[CAN]
BitRate=1M
```

### (Optional) Add static config to systemd-networkd
Make file `/etc/systemd/network/20-wired.network`. Change Name, Address, Gateway, and DNS as needed.
```
[Match]
Name=usb0

[Link]
RequiredForOnline=no

[Network]
Address=10.10.1.2/24
Gateway=10.10.1.1
DNS=10.10.1.1
DNS=8.8.8.8
```

### Enable systemd-networkd
`sudo systemctl enable systemd-networkd`
`sudo systemctl enable systemd-resolved` If using static config.

### Add override.conf to /etc/systemd/system/systemd-networkd-wait-online.service.d
```
[Service]
ExecStart=
ExecStart=/lib/systemd/systemd-networkd-wait-online --interface=can1
```

[systemd-networkd]:https://wiki.archlinux.org/index.php/Systemd-networkd
