#!/bin/bash


if [ $# -eq 0 ]; then
    echo "No arguments supplied"
    echo "./build_images <BOARD>"
    echo "where <BOARD> can be:"
    echo "  generic"
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

# build partitions
./image-builder/RootStock-NG.sh -c $BOARD

cp ./configs/*.conf ./image-builder/configs/# make .img file
./image-builder/deploy/debian-*/setup_sdcard.sh --img-2gb startracker --dtb beaglebone

# make tar
xz -z -8 -v ./image-builder/deploy/debian-*/*.img
