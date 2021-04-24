#!/bin/sh -e

##############################################################################
echo "disable root login"

echo "PermitRootLogin prohibit-password" >> /etc/ssh/sshd_config

##############################################################################
echo "set default boot power mode"

cat > "/etc/default/cpufrequtils" <<-__EOF__
GOVERNOR="powersave"
__EOF__

##############################################################################
echo "setjournald configs"

cat > "/etc/systemd/journald.conf" <<-__EOF__
[Journal]
SystemMaxUse=50M
__EOF__

##############################################################################
echo "setup serial?"

echo "" >> /etc/securetty
echo "#USB Gadget Serial Port" >> /etc/securetty
echo "ttyGS0" >> /etc/securetty

##############################################################################
echo "setup usb ethernet"

cat > "/etc/default/bb-boot" <<-__EOF__
USB_NETWORK_DISABLED=yes
USB_CONFIGURATION=enable
__EOF__

echo "g_ether" > /etc/modules-load.d/g_ether.conf
HOST_ADDR=`dmesg | tr -s " " | grep "usb0: HOST MAC" | cut -d " " -f 6`
echo "options g_ether host_addr=$HOST_ADDR" > /etc/modprobe.d/g_ether.conf

##############################################################################
echo "setup systemd-networkd"

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
Address=192.168.6.2/24
Gateway=192.168.6.1
DNS=192.168.6.1
DNS=8.8.8.8
DNS=8.8.4.4
__EOF__

# enable systemd-networkd on boot
systemctl enable systemd-networkd
systemctl enable systemd-resolved

##############################################################################
echo "remove internet packages required during build"

apt -y purge git git-man curl wget rsync ifupdown
