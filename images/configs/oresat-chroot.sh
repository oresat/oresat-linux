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
# journald configs

cat > "/etc/systemd/journald.conf" <<-__EOF__
[Journal]
SystemMaxUse=50M
__EOF__

##############################################################################
# set locales so PAM doesn't log a critial error every hour

localectl set-locale LANG=en_US.UTF-8

##############################################################################
# serial

echo "" >> /etc/securetty
echo "#USB Gadget Serial Port" >> /etc/securetty
echo "ttyGS0" >> /etc/securetty

##############################################################################
# setup usb ethernet

cat > "/etc/default/bb-boot" <<-__EOF__
#USB_NETWORK_DISABLED=yes
USB_CONFIGURATION=enable
__EOF__

echo "g_ether" > /etc/modules-load.d/g_ether.conf
HOST_ADDR=`dmesg | tr -s " " | grep "usb0: HOST MAC" | cut -d " " -f 6`
echo "options g_ether host_addr=$HOST_ADDR" > /etc/modprobe.d/g_ether.conf

##############################################################################
# setup systemd-networkd for CAN

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
