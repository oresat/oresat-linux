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
    echo "  generic"
    echo "  gps"
    echo "  star-tracker"
    exit 1
fi

NODE_ID=`grep "^$1=" node_ids.txt | cut -d "=" -f 2`
echo $NODE_ID

BOARD="oresat-"$1
echo $BOARD

DATE=`date "+%F"`
NAME="$BOARD-$DATE"

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
for filename in `ls device_trees_overlays/*.dts`; do
    cp bb.org-overlays/src/arm/$(basename "$filename" .dts).dtbo device_trees_overlays/
done

# copy oresat config into correct dirs
cp ./configs/*.conf ./image-builder/configs/
cp ./chroot_scripts/*.sh ./image-builder/target/chroot/
cp ./uEnv/*.txt ./image-builder/target/boot/

# add olm config
echo "\

##############################################################################
echo \"adding OLM config\"

cat > \"/etc/oresat-linux-manager.conf\" <<-__EOF__
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

# make .img file
sudo ./setup_sdcard.sh $NAME.img --dtb beaglebone --enable-mainline-pru-rproc

# compress
zstd $NAME.img

cd ../../..

mv image-builder/deploy/debian-*/$NAME.img.zst .
