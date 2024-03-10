#!/bin/bash -e

list="c3 cfc dev dxwifi gps star-tracker flight"

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

# build u-boot
make -C ../uboot

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
sudo ./setup_sdcard.sh --img-$SIZE $NAME.img --dtb beaglebone --enable-fat-partition

# compress
zstd $NAME-$SIZE.img

cd ../../..

mv image-builder/deploy/debian-*/$NAME-$SIZE.img.zst .

# generate sha256
sha256sum $NAME-$SIZE.img.zst > $NAME-$SIZE.img.zst.sha256
