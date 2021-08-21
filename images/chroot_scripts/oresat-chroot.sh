#!/bin/sh -e

##############################################################################
echo "disable root login"

echo "PermitRootLogin prohibit-password" >> /etc/ssh/sshd_config

##############################################################################
echo "set default boot power mode"

if [ hostname != "dev" ]; then
cat > "/etc/default/cpufrequtils" <<-__EOF__
GOVERNOR="powersave"
__EOF__
else
cat > "/etc/default/cpufrequtils" <<-__EOF__
GOVERNOR="performance"
__EOF__
fi

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
echo "setup systemd-networkd"

cat > "/etc/systemd/network/10-can.network" <<-__EOF__
[Match]
Name=can1

[CAN]
BitRate=1M
__EOF__

cat > "/etc/systemd/network/20-wired-usb0.network" <<-__EOF__
[Match]
Name=usb0

[Network]
Address=192.168.7.2/24
Gateway=192.168.7.1
DNS=192.168.7.1
DNS=8.8.8.8
DNS=8.8.4.4
__EOF__

cat > "/etc/systemd/network/20-wired-usb1.network" <<-__EOF__
[Match]
Name=usb1

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

if [ hostname != "dev" ]; then
apt -y purge git git-man curl wget rsync
fi
