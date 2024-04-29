#!/bin/sh -e

HOSTNAME=`cat /etc/hostname`

##############################################################################
echo "disable root login"

echo "PermitRootLogin prohibit-password" >> /etc/ssh/sshd_config

##############################################################################
echo "enable g_ether (ethernet over usb)"

MAC_ADDR_BASE="60:64:05:f9:0d"
if [ $HOSTNAME == "oresat-c3" ]; then
MAC_ADDR="$MAC_ADDR_BASE:10"
elif [ $HOSTNAME == "oresat-cfc" ]; then
MAC_ADDR="$MAC_ADDR_BASE:20"
elif [ $HOSTNAME == "oresat-dxwifi" ]; then
MAC_ADDR="$MAC_ADDR_BASE:30"
elif [ $HOSTNAME == "oresat-gps" ]; then
MAC_ADDR="$MAC_ADDR_BASE:40"
elif [ $HOSTNAME == "oresat-star-tracker" ]; then
MAC_ADDR="$MAC_ADDR_BASE:50"
else
MAC_ADDR="$MAC_ADDR_BASE:f0"
fi

echo "g_ether" > /etc/modules-load.d/g_ether.conf
echo "options g_ether host_addr=$HOST_ADDR" > /etc/modprobe.d/g_ether.conf

##############################################################################
echo "add OreSat OLAF app daemon"

# add config
cat > "/etc/systemd/system/"$HOSTNAME"d.service" <<-__EOF__
[Unit]
Description=OreSat Linux App

[Service]
Type=simple
ExecStart=/usr/local/bin/$HOSTNAME -b can0 -l
Restart=always
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
if [ $HOSTNAME != "oresat-dev" ] && [ $HOSTNAME != "oresat-generic" ]; then
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

systemctl enable nginx.service

##############################################################################
echo "enable SPIDEV kernel module at boot"

cat > "/etc/modules-load.d/spidev.conf" <<-__EOF__
spidev
__EOF__

##############################################################################
echo "add systemd-networkd configs"

cat > "/etc/systemd/network/10-usb0.link" <<-__EOF__
[Match]
OriginalName=usb0

[Link]
MACAddress=$MAC_ADDR
__EOF__

cat > "/etc/systemd/network/20-wired.network" <<-__EOF__
[Match]
Name=usb0 eth0

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
echo "rebuild pyyaml"

python3 -m pip install --force-reinstall --no-cache-dir --no-binary pyyaml pyyaml

##############################################################################
echo "add oresat device trees to /boot"

dtb_dir=`ls -d /boot/dtbs/*`
mv /tmp/*.dtb $dtb_dir
chmod 755 $dtb_dir/oresat*

##############################################################################
# Flight images only

if [ $HOSTNAME != "oresat-dev" ]; then
echo "remove internet packages required during build"
apt -y purge git git-man curl wget rsync

echo "disable timesyncd"
systemctl disable systemd-timesyncd.service
fi

echo "install deb packages"
if [ "$HOSTNAME" = "oresat-c3" ]; then
    dpkg -i /tmp/ax5043_* /tmp/oresat-c3-watchdog_*
elif [ "$HOSTNAME" = "oresat-cfc" ]; then
    dpkg -i /tmp/prucam-*
elif [ "$HOSTNAME" = "oresat-star-tracker" ]; then
    dpkg -i /tmp/prucam-*
elif [ "$HOSTNAME" = "oresat-dxwifi" ]; then
    dpkg -i /tmp/oresat-dxwifi-*
fi
rm -f /tmp/*.deb
