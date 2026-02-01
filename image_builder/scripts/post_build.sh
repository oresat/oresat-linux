#!/bin/bash -e

if [ "$(id -u)" != "0" ]; then
  echo "Log: (post-build) need to be root"
  exit 1
fi

target_dir="fs"
root_fs="fs/fs-rootfs"
boot_fs="fs/fs-bootfs"
dtbs_path=""
fdt=""

# these are supplied by the project config file
rfs_username=""
rfs_password=""
rfs_hostname=""
config_path="./image-builder.project"

# load build configuration
# exit if not present
if [ ! -f image-builder.project ]; then
  echo "ERROR: (post-build) missing .project configuration"
  exit 1
fi

# shellcheck disable=SC1090
. "${config_path}"

mkdir -p "${target_dir}"

if [ ! -f "MLO" ]; then
  echo "ERROR: (post-build) MLO missing"
  exit 1
fi

cp -v MLO "${target_dir}"

if [ ! -f "u-boot-dtb.img" ]; then
  echo "ERROR: (post-build) u-boot-dtb.img missing"
  exit 1
fi

cp -v u-boot-dtb.img "${target_dir}"

echo "Log: (post-build) building root fs"
mkdir -p "${root_fs}"
tar xf armhf-rootfs-debian-*.tar -C "${root_fs}"

dir_check="${root_fs}/boot"
kernel_select() {
  echo "debug: kernel_select: picking the first available kernel..."
  unset check
  check=$(ls "${dir_check}" | grep vmlinuz- | head -n 1)
  if [ "${check}" != "" ]; then
    kernel_version=$(ls "${dir_check}" | grep vmlinuz- | head -n 1 | awk -F'vmlinuz-' '{print $2}')
    echo "debug: kernel_select: found: [${kernel_version}]"
  else
    echo "ERROR: no installed kernel"
    exit
  fi
}

# get the first available kernel
kernel_select

# set up uenv
echo "Log: (post-build) building boot fs"
echo "uname_r=${kernel_version}" >>"${root_fs}/boot/uEnv.txt"
echo "cmdline=fsck.repair=yes earlycon net.ifnames=0" >>"${root_fs}/boot/uEnv.txt"

mkdir -p "${boot_fs}"
cat <<__EOF__ >"${boot_fs}/sysconf.txt"
user_name="${rfs_username}"
user_password="${rfs_password}"
hostname="${rfs_hostname}"
__EOF__

cp -v "${root_fs}/boot/vmlinuz-${kernel_version}" "${boot_fs}"
cp -v "${root_fs}/boot/initrd.img-${kernel_version}" "${boot_fs}"

echo "Log: (post-build) copying fdt"
mkdir -p "${boot_fs}/dtbs/${kernel_version}"

shopt -s nullglob
if [ "${rfs_hostname}" != "oresat-dev" ]; then
  dtbs_path=("${root_fs}/boot/dtbs/${kernel_version}/${rfs_hostname}"-*.dtb)
  fdt="${dtbs_path[0]##*/}"
else
  fdt="am335x-boneblack.dtb"
fi

echo "Log: (post-build) configuring extlinux"
cp -v "${root_fs}/boot/dtbs/${kernel_version}/${fdt}" "${boot_fs}/dtbs/${kernel_version}"

mkdir -p "${boot_fs}/extlinux"
cat <<__EOF__ >"${boot_fs}/extlinux/extlinux.conf"
TIMEOUT 1
DEFAULT linux

LABEL linux
KERNEL /vmlinuz-${kernel_version}
INITRD /initrd.img-${kernel_version}
FDT /dtbs/${kernel_version}/${fdt}
APPEND console=ttyS0,115200 root=/dev/mmcblk0p2 rw rootwait
__EOF__

if [ ! -f ./genimage.cfg ]; then
  echo "ERROR: (post-build) genimage configuration missing"
  exit 1
fi

genimage --rootpath "${target_dir}" --inputpath "${target_dir}" --config genimage.cfg
