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

cd image-builder

# build partitions
./RootStock-NG.sh -c $BOARD

./deploy/debian-*/setup_sdcard.sh --img-2gb startracker --dtb beaglebone

# make tar
xz -z -8 -v ./deploy/debian-*/*.img

cd ..
