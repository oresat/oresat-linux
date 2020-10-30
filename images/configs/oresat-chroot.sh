#!/bin/sh -e

##############################################################################
# disable root login

echo "PermitRootLogin prohibit-password" >> /etc/ssh/sshd_config

##############################################################################
# set default boot power mode

cat > "/etc/default/cpufrequtils" <<-__EOF__
GOVERNOR="powersave"
__EOF__

##############################################################################
# setup usb ethernet

echo "" >> /etc/securetty
echo "#USB Gadget Serial Port" >> /etc/securetty
echo "ttyGS0" >> /etc/securetty

#cat > "/etc/default/bb-boot" <<-__EOF__
#USB_CONFIGURATION=enable
#USB_NETWORK_DISABLED="yes"
#__EOF__

#echo "g_ether" > /etc/modules-load.d/g_ether.conf
#HOST_ADDR=`dmesg | grep "usb0: HOST MAC" | cut -d " " -f 8`
#echo "options g_ether host_addr=$HOST_ADDR" > /etc/modprobe.d/g_ether.conf

##############################################################################
# setup systemd-networkd

cat > "/etc/systemd/network/10-can.network" <<-__EOF__
[Match]
Name=can1

[CAN]
BitRate=1M
__EOF__

cat > "/etc/systemd/network/20-wired.network" <<-__EOF__
[Match]
Name=usb0

[Network]
Address=168.192.6.2/24
Gateway=168.192.6.1
DNS=168.192.6.1
DNS=8.8.8.8
DNS=8.8.4.4
__EOF__

# enable systemd-networkd on boot
systemctl enable systemd-networkd
systemctl enable systemd-resolved

##############################################################################
# remove internet packages required during build

apt -y purge git git-man curl wget rsync

##############################################################################
# remove things we don't want that are install

apt -y purge rtl8723bu-modules-`uname -r` rtl8821cu-modules-`uname -r` nano nano-tiny
