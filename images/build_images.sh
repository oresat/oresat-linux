#!/bin/bash

if [ $# -eq 0 ]; then
    echo "No arguments supplied"
    echo "./build_images <BOARD>"
    echo "where <BOARD> can be:"
    echo "  generic"
    echo "  gps"
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

cd image-builder

# build partitions
./RootStock-NG.sh -c $BOARD

cd  deploy/debian-*/

#must be done as root
if [ $BOARD == "oresat-generic" ]; then
    ./setup_sdcard.sh --img-1gb $BOARD-`date "+%F"`.img --dtb beaglebone
else
    ./setup_sdcard.sh --img-2gb $BOARD-`date "+%F"`.img --dtb beaglebone
fi

cd ../../..
