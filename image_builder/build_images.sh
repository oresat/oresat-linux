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

BOARD="oresat-"$1
DATE=`date "+%F"`
NAME="$BOARD-$DATE"
SIZE="2gb"

if [ $BOARD == "oresat-dev" ]; then
    SIZE="4gb"
fi

if [ ! -d image-builder ]; then
    git clone https://github.com/beagleboard/image-builder
fi

# build all device trees
make -C ../device_trees

#  if [ $BOARD != "oresat-dev" ] && [ $BOARD != "oresat-generic" ]; then
# DTB=`ls ../device_trees/$BOARD-*.dtb | tail -n1`
# SETUP_SDCARD_EXTRA_ARGS=" \
#   --enable-uboot-disable-pru \
#   --enable-extlinux \
#   --enable-extlinux-fdtdir \
#   --enable-extlinux-append"
# #  --force-device-tree $DTB"
# else
# SETUP_SDCARD_EXTRA_ARGS=" \
#   --enable-extlinux \
#   --enable-extlinux-fdtdir \
#   --enable-extlinux-append"
#
# fi

SETUP_SDCARD_EXTRA_ARGS=" \
  --enable-extlinux \
  --enable-extlinux-fdtdir \
  --enable-extlinux-append"

if [ $BOARD != "oresat-dev" ] && [ $BOARD != "oresat-generic" ]; then
#DTB=`ls ../device_trees/$BOARD-*.dtb | tail -n1`
SETUP_SDCARD_EXTRA_ARGS="--enable-uboot-disable-pru ${SETUP_SDCARD_EXTRA_ARGS}"
fi

echo $SETUP_SDCARD_EXTRA_ARGS

# copy oresat config into correct dirs
cp ./configs/*.conf ./image-builder/configs/
cp ./chroot_scripts/*.sh ./image-builder/target/chroot/
cp ./uEnv/*.txt ./image-builder/target/boot/ #disappearing

cd image-builder

# clear any previous builds
rm -rf deploy

# build partitions
./RootStock-NG.sh -c $BOARD

cd deploy/debian-*/
cp ../../../setup_sdcard.sh .

# make .img file
sudo ./setup_sdcard.sh --img-$SIZE $NAME.img --dtb beaglebone $SETUP_SDCARD_EXTRA_ARGS

# compress
zstd $NAME-$SIZE.img

cd ../../..

mv image-builder/deploy/debian-*/$NAME-$SIZE.img.zst .

# generate sha256
sha256sum $NAME-$SIZE.img.zst > $NAME-$SIZE.img.zst.sha256
