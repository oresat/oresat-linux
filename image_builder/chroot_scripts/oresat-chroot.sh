#!/bin/sh -e

HOSTNAME=`cat /etc/hostname`

##############################################################################
echo "disable root login"

echo "PermitRootLogin prohibit-password" >> /etc/ssh/sshd_config

##############################################################################
echo "add OreSat OLAF app daemon"

if [ $HOSTNAME != "oresat-dev" ]; then
# add config
cat > "/etc/systemd/system/"$HOSTNAME"d.service" <<-__EOF__
[Unit]
Description=OreSat Linux App

[Service]
Type=simple
ExecStart=/usr/local/bin/$HOSTNAME -b can1 -l
Restart=on-failure
User=root
Group=root
CPUQuota=100%
CPUWeight=1000

[Install]
WantedBy=multi-user.target
Alias=oresatd.service
__EOF__

# enable daemon
systemctl daemon-reload
systemctl enable $HOSTNAME"d.service"
fi

##############################################################################
echo "setup and configure nginx for OreSat OLAF app"

cat > "/etc/nginx/nginx.conf" <<-__EOF__
events {
  worker_connections 768;
}

http {
  server {
	  listen 80;
	  server_name _;

    location / {
      proxy_pass http://127.0.0.1:8000/;
      proxy_set_header X-Forwarded-For \$proxy_add_x_forwarded_for;
      proxy_set_header X-Forwarded-Proto \$scheme;
      proxy_set_header X-Forwarded-Host \$host;
      proxy_set_header X-Forwarded-Prefix /;
    }
  }
}
__EOF__

# only enable on flight images
if [ $HOSTNAME != "oresat-dev" ]; then
systemctl enable nginx.service
fi

##############################################################################
echo "enable SPIDEV kernel module at boot"

cat > "/etc/modules-load.d/spidev.conf" <<-__EOF__
spidev
__EOF__

##############################################################################
echo "replace BeagleBoard's systemd-networkd config"

# disable networking script
cat > "/etc/default/bb-boot" <<-__EOF__
USB_CONFIGURATION=disable
#USB_NETWORK_DISABLED=yes
__EOF__

# remove BeagleBoard's configs
rm /etc/systemd/network/*

# usb0 is for Window hosts
# usb1 is for *nix hosts
# eth0 is for the BeagleBone Black's ethernet port
cat > "/etc/systemd/network/20-wired.network" <<-__EOF__
[Match]
Name=usb0 usb1 eth0

[Link]
RequiredForOnline=no

[Network]
DHCP=yes
MulticastDNS=yes
__EOF__

# make sure these are enabled
systemctl enable systemd-networkd.service
systemctl enable systemd-resolved.service

##############################################################################
echo "remove internet packages required during build on flight images"

if [ $HOSTNAME != "oresat-dev" ]; then
apt -y purge git git-man curl wget rsync
fi
