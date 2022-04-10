#!/bin/bash

if [ $# -ne 1 ]; then
    echo "Invalid arguments"
    echo ""
    echo "./build_images <BOARD>"
    echo ""
    echo "where <BOARD> can be:"
    echo "  cfc"
    echo "  dev"
    echo "  dxwifi"
    echo "  gps"
    echo "  star-tracker"
    exit 1
fi

BOARD="oresat-"$1
DATE=`date "+%F"`
NAME="$BOARD-$DATE"

if [ ! -d image-builder ]; then
    git clone https://github.com/beagleboard/image-builder
fi

# copy oresat config into correct dirs
cp ./configs/*.conf ./image-builder/configs/
cp ./chroot_scripts/*.sh ./image-builder/target/chroot/
cp ./uEnv/*.txt ./image-builder/target/boot/

cd image-builder

# clear any previous builds
rm -rf deploy

# build partitions
./RootStock-NG.sh -c $BOARD

cd deploy/debian-*/

# make .img file
sudo ./setup_sdcard.sh --img-2gb $NAME.img --dtb beaglebone --enable-mainline-pru-rproc

# compress
zstd $NAME-2gb.img

cd ../../..

mv image-builder/deploy/debian-*/$NAME-2gb.img.zst .
