#!/bin/bash

if [ $# -ne 1 ]; then
    echo "Invalid arguments"
    echo ""
    echo "./build_images <BOARD>"
    echo ""
    echo "where <BOARD> can be:"
    echo "  cfc"
    echo "  dxwifi"
    echo "  generic"
    echo "  gps"
    echo "  star-tracker"
    exit 1
fi

NODE_ID=`grep "^$1=" node_ids.txt | cut -d "=" -f 2`
echo $NODE_ID

BOARD="oresat-"$1
echo $BOARD

if [ ! -d image-builder ]; then
    git clone https://github.com/beagleboard/image-builder
fi

# copy oresat config into correct dirs
cp ./configs/*.conf ./image-builder/configs/
cp ./configs/*.sh ./image-builder/target/chroot/
cp ./configs/*.txt ./image-builder/target/boot/

echo "\

##############################################################################
echo \"adding OLM config\"

cat >> \"/etc/oresat-linux-manager.conf\" <<-__EOF__
[Bus]
Name=can1

[Node]
ID=$NODE_ID
__EOF__" >> image-builder/target/chroot/oresat-chroot.sh

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
