#!/bin/bash

if [ $# -eq 0 ]; then
    echo "No arguments supplied"
    echo "./build_images <BOARD>"
    echo "where <BOARD> can be:"
    echo "  cfc"
    echo "  generic"
    echo "  gps"
    echo "  live"
    echo "  startracker"
    exit 1
fi

BOARD="oresat-"$1
echo $BOARD

if [ ! -d image-builder ]; then
    git clone https://github.com/beagleboard/image-builder
fi

# copy oresat config into correct dirs
cp ./configs/*.conf ./image-builder/configs/
cp ./configs/*.sh ./image-builder/target/chroot/
cp ./configs/*.txt ./image-builder/target/boot/

# override setup_sdcard script
cp ./setup_sdcard.sh ./image-builder/tools/

cd image-builder

# clear any previous builds
rm -rf deploy

# build partitions
./RootStock-NG.sh -c $BOARD

cd deploy/debian-*/

# .img file
sudo ./setup_sdcard.sh $BOARD-`date "+%F"`.img --dtb beaglebone --enable-mainline-pru-rproc

cd ../../..
