#!/bin/bash -e
# this script is run in the target context

rfs_hostname=""
qemu_command=""
# source project configuration passed
# into target by image-builder/scripts/chroot.sh
if [ -f /etc/rcn-ee.conf ]; then
  # shellcheck disable=SC1091
  . /etc/rcn-ee.conf
fi

if [ -f /etc/oib.project ]; then
  # shellcheck disable=SC1091
  . /etc/oib.project
fi

is_this_qemu() {
  unset warn_qemu_will_fail
  if [ -f /usr/bin/qemu-arm-static ]; then
    warn_qemu_will_fail=1
  fi
}

qemu_warning() {
  if [ "${warn_qemu_will_fail}" ]; then
    echo "Log: (chroot) Warning, qemu can fail here... (run on real armv7l hardware for production images)"
    echo "Log: (chroot): [${qemu_command}]"
  fi
}

##############################################################################
is_this_qemu
qemu_warning

echo "Log: (chroot) disable root login"

echo "PermitRootLogin prohibit-password" >>/etc/ssh/sshd_config

##############################################################################
echo " Log: (chroot) enable g_ether (ethernet over usb)"

mac_addr_base="60:64:05:f9:0d"
case "${rfs_hostname}" in
"oresat-c3")
  mac_addr="${mac_addr_base}:10"
  ;;

"oresat-cfc")
  mac_addr="${mac_addr_base}:20"
  ;;

"oresat-dxwifi")
  mac_addr="${mac_addr_base}:30"
  ;;

"oresat-gps")
  mac_addr="${mac_addr_base}:40"
  ;;

"oresat-star-tracker")
  mac_addr="${mac_addr_base}:50"
  ;;

*)
  mac_addr="${mac_addr_base}:f0"
  ;;
esac

echo "g_ether" >/etc/modules-load.d/g_ether.conf
echo "options g_ether host_addr=${mac_addr}" >/etc/modprobe.d/g_ether.conf

##############################################################################
echo "Log: (chroot) add OreSat OLAF app daemon"

# add config
cat <<__EOF__ >"/etc/systemd/system/${rfs_hostname}d.service"
[Unit]
Description=OreSat Linux App

[Service]
Type=simple
ExecStart=/usr/local/bin/${rfs_hostname} -b can0 -l
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
if [ "${rfs_hostname}" != "oresat-dev" ] && [ "${rfs_hostname}" != "oresat-generic" ]; then
  systemctl enable "${rfs_hostname}d.service"
fi

##############################################################################
echo "Log: (chroot) add growparts oneshot daemon"

cat <<__EOF__ >"/etc/systemd/system/grow-partition.service"
[Unit]
Description=Grow active root partition

[Service]
Type=oneshot
ExecStart=bash /opt/scripts/grow_partition.sh
ExecStart=systemctl disable grow-partition
User=root
Group=root

[Install]
WantedBy=multi-user.target
__EOF__

systemctl daemon-reload
systemctl enable grow-partition.service

##############################################################################
echo "Log: (chroot) setup and configure nginx for OreSat OLAF app"

cat <<__EOF__ >"/etc/nginx/nginx.conf"
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
echo "Log: (chroot) enable SPIDEV kernel module at boot"

cat <<__EOF__ >"/etc/modules-load.d/spidev.conf"
spidev
__EOF__

##############################################################################
echo "Log: (chroot) add systemd-networkd configs"

cat <<__EOF__ >"/etc/systemd/network/10-usb0.link"
[Match]
OriginalName=usb0

[Link]
RequiredForOnline=no
MACAddress=${mac_addr}
__EOF__

cat <<__EOF__ >"/etc/systemd/network/10-usb0.network"
[Match]
Name=usb0

[Network]
DHCP=yes
MulticastDNS=yes
__EOF__

cat <<__EOF__ >"/etc/systemd/network/10-eth0.link"
[Match]
Name=eth0

[Link]
RequiredForOnline=no 
__EOF__

cat <<__EOF__ >"/etc/systemd/network/10-eth0.network"
[Match]
Name=eth0

[Network]
DHCP=yes
MulticastDNS=yes
__EOF__

# make sure these are enabled
systemctl enable systemd-networkd.service
# systemctl enable systemd-resolved.service

##############################################################################
#echo "Log: (chroot) rebuild pyyaml"

#python3 -m pip install --force-reinstall --no-cache-dir --no-binary pyyaml pyyaml

##############################################################################
echo "Log: (chroot) add oresat device trees to /boot"

dtb_dirs=(/boot/dtbs/*)

# get first available dtb dir
dtb_dir="${dtb_dirs[0]}"
mv /tmp/*.dtb "${dtb_dir}"
chmod 755 "${dtb_dir}"/oresat*

if [ "${rfs_hostname}" != "oresat-dev" ] && [ "${rfs_hostname}" != "oresat-generic" ]; then
  echo "Log: (chroot) replacing pocketbeagle dt with latest custom card dt"

  dt_path=$(ls -d /boot/dtbs/*)
  dtbs=("${dt_path}/${rfs_hostname}"-*.dtb)
  dt="${dtbs[-1]}"

  # back up original pocketbeagle dtb
  mv "${dt_path}"/am335x-pocketbeagle.dtb "${dt_path}"/am335x-pocketbeagle.dtb-orig
  ln -s "${dt}" "${dt_path}"/am335x-pocketbeagle.dtb
fi

##############################################################################
# Card unique changes

if [ "${rfs_hostname}" = "oresat-star-tracker" ]; then
  lost_whl="lost-0.0.0-py3-none-any.whl"
  wget https://packages.oresat.org/python/"${lost_whl}"
  pip3 install "${lost_whl}"
  rm -f "${lost_whl}"
fi

##############################################################################
# Flight images only

if [ "${rfs_hostname}" != "oresat-dev" ]; then
  echo "remove internet packages required during build"
  apt -y purge git git-man curl wget rsync

  echo "disable timesyncd"
  systemctl disable systemd-timesyncd.service
fi
