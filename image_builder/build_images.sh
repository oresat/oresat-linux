#!/bin/bash -e

list="c3 cfc dev dxwifi generic gps star-tracker"

if [[ ! $list =~ (^|[[:space:]])$1($|[[:space:]]) ]]; then
  echo "Invalid board argument"
  echo ""
  echo "./build_images.sh <BOARD>"
  echo ""
  echo "where <BOARD> can be:"
  for i in $list; do
    echo "  $i"
  done
  exit 1
fi

DIR="$PWD"

board="oresat-"$1
date=$(date "+%F")
name="${board}-${date}"
size="2gb"

if [ "${board}" == "oresat-dev" ]; then
  size="4gb"
fi

bootloader_dir="u-boot"
spl="MLO"
bootloader="u-boot-dtb.img"
dtb="oresat-bootloader"
image_dir="images"

# copy oresat config into correct dirs
cp -v configs/"${board}".conf image-builder/configs
cp -v configs/"${dtb}".conf image-builder/tools/hwpack
cp -v chroot_scripts/oresat-chroot.sh image-builder/target/chroot
cp -v chroot_scripts/oresat-early-chroot.sh image-builder/target/chroot

cd image-builder

# clear any previous builds
rm -rf deploy

# build partitions
/bin/bash -e RootStock-NG.sh -c "${board}"

cd deploy/debian-*/
cp -v "${DIR}/scripts/post_build.sh" .
cp -v "${DIR}/configs/genimage.cfg" .
cp -v "${DIR}/${bootloader_dir}/${spl}" .
cp -v "${DIR}/${bootloader_dir}/${bootloader}" .

# make .img file
/bin/bash -e post_build.sh

# compress
mv images/sdcard.img "${name}-${size}.img"
zstd "${name}-${size}.img"

mkdir -p "${DIR}/${image_dir}"

# give ownership to non-root users
chgrp --recursive users "${DIR}/${image_dir}"
chmod --recursive g+w "${DIR}/${image_dir}"

mv "${name}-${size}.img.zst" "${DIR}/${image_dir}"
cd "${DIR}/${image_dir}"

# generate sha256
sha256sum "${name}-${size}.img.zst" >"${name}-${size}.img.zst.sha256"
