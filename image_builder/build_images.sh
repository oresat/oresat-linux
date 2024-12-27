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

BOOTLOADER_DIR="u-boot"
SPL="MLO"
BOOTLOADER="u-boot-dtb.img"
DTB="oresat-bootloader"
IMAGE_DIR="images"

SETUP_SDCARD_EXTRA_ARGS=" \
    --spl $BOOTLOADER_DIR/$SPL \
    --bootloader $BOOTLOADER_DIR/$BOOTLOADER \
"

if [ $BOARD != "oresat-dev" ] && [ $BOARD != "oresat-generic" ]; then
    SETUP_SDCARD_EXTRA_ARGS="--enable-uboot-disable-pru ${SETUP_SDCARD_EXTRA_ARGS}"
fi

echo "setup_sdcard.sh options: $SETUP_SDCARD_EXTRA_ARGS"

# copy oresat config into correct dirs
cp ./configs/oresat-*.conf ./image-builder/configs/
cp ./configs/$DTB.conf ./image-builder/tools/hwpack
cp ./chroot_scripts/*.sh ./image-builder/target/chroot/

cd image-builder

# clear any previous builds
rm -rf deploy

# build partitions
./RootStock-NG.sh -c $BOARD

cd deploy/debian-*/
cp ../../../$BOOTLOADER_DIR/{$SPL,$BOOTLOADER} ./u-boot

# make .img file
sudo ./setup_sdcard.sh --img-$SIZE $NAME.img --dtb $DTB $SETUP_SDCARD_EXTRA_ARGS

# compress
zstd $NAME-$SIZE.img

cd ../../..

mkdir -p $IMAGE_DIR

mv image-builder/deploy/debian-*/$NAME-$SIZE.img.zst $IMAGE_DIR

# generate sha256
sha256sum $IMAGE_DIR/$NAME-$SIZE.img.zst > $IMAGE_DIR/$NAME-$SIZE.img.zst.sha256
