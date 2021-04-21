#!/bin/bash

if [ $# -eq 0 ]; then
    echo "No arguments supplied"
    echo "./build_images <BOARD>"
    echo "where <BOARD> can be:"
    echo "  cfc"
    echo "  dxwifi"
    echo "  generic"
    echo "  gps"
    echo "  star-tracker"
    exit 1
fi

BOARD="oresat-"$1
echo $BOARD

if [ ! -d image-builder ]; then
    git clone https://github.com/beagleboard/image-builder
fi

if [ ! -d bb.org-overlays ]; then
    git clone https://github.com/beagleboard/bb.org-overlays
fi

cp ./device_trees_overlays/*.dts bb.org-overlays/src/arm/

# build dtbo
cd bb.org-overlays
make
cd ..

# copy all new dtbo
for filename in `ls device_trees_overlays`
do
  cp -- bb.org-overlays/src/arm/"$filename" device_trees_overlays/"$(basename -- "$filename" .dts).dtbo"
done

# copy oresat config into correct dirs
cp ./configs/*.conf ./image-builder/configs/
cp ./chroot_script/*.sh ./image-builder/target/chroot/
cp ./uEnv/*.txt ./image-builder/target/boot/

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
