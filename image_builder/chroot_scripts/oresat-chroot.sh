#!/bin/sh -e

##############################################################################
echo "disable root login"

echo "PermitRootLogin prohibit-password" >> /etc/ssh/sshd_config

##############################################################################
echo "set default boot power mode"

HOSTNAME=`cat /etc/hostname`
if [ $HOSTNAME != "oresat-dev" ]; then
cat > "/etc/default/cpufrequtils" <<-__EOF__
GOVERNOR="powersave"
__EOF__
else
cat > "/etc/default/cpufrequtils" <<-__EOF__
GOVERNOR="performance"
__EOF__
fi

##############################################################################
echo "add OreSat app daemon"

if [ $HOSTNAME != "oresat-dev" ]; then
# add config
cat > "/etc/systemd/system/"$HOSTNAME"d.service" <<-__EOF__
[Unit]
Description=OreSat Linux App
Wants=network-online.target
After=network-online.target

[Service]
Type=simple
ExecStart=/usr/bin/"$HOSTNAME" -b can1 -l
Restart=on-failure
User=root
Group=root

[Install]
WantedBy=multi-user.target
Alias=oresatd.service
__EOF__

# enable daemon
systemctl daemon-reload
systemctl enable $HOSTNAME"d.service"
fi

##############################################################################
echo "replace BeagleBoard's systemd-networkd config"

# disable networking script
cat > "/etc/default/bb-boot" <<-__EOF__
USB_CONFIGURATION=disable
USB_NETWORK_DISABLED_yes
__EOF__

# remove BeagleBoard's configs
rm /etc/systemd/network/*

cat > "/etc/systemd/network/10-can.network" <<-__EOF__
[Match]
Name=can*

[Link]
RequiredForOnline=no

[CAN]
BitRate=1M
__EOF__

# usb0 (for Window host)
# usb1 (for *nix host)
# eth0 (for BeagleBone Black ethernet port)
cat > "/etc/systemd/network/20-wired.network" <<-__EOF__
[Match]
Name=usb0 usb1 eth0

[Link]
RequiredForOnline=no

[Network]
DHCP=yes
__EOF__

##############################################################################
echo "remove internet packages required during build on flight images"

if [ $HOSTNAME != "oresat-dev" ]; then
apt -y purge git git-man curl wget rsync
fi
