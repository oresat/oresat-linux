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

ORESAT_BOOTLOADER_DIR="oresat_bootloader"

# Initialize submodules, patch image_builder,and build uboot
make

# build all device trees
make -C ../device_trees

SETUP_SDCARD_EXTRA_ARGS=" \
    --enable-extlinux \
    --spl $ORESAT_BOOTLOADER_DIR/MLO \
    --bootloader $ORESAT_BOOTLOADER_DIR/u-boot-dtb.img \
"

if [ $BOARD != "oresat-dev" ] && [ $BOARD != "oresat-generic" ]; then
    SETUP_SDCARD_EXTRA_ARGS="--enable-uboot-disable-pru ${SETUP_SDCARD_EXTRA_ARGS}"
fi

echo "setup_sdcard.sh options: $SETUP_SDCARD_EXTRA_ARGS"

# copy oresat config into correct dirs
cp ./configs/*.conf ./image-builder/configs/
cp ./chroot_scripts/*.sh ./image-builder/target/chroot/

cd image-builder

# clear any previous builds
rm -rf deploy

# build partitions
./RootStock-NG.sh -c $BOARD

cd deploy/debian-*/
cp -r ../../../$ORESAT_BOOTLOADER_DIR .

# make .img file
sudo ./setup_sdcard.sh --img-$SIZE $NAME.img --dtb beaglebone $SETUP_SDCARD_EXTRA_ARGS

# compress
zstd $NAME-$SIZE.img

cd ../../..

IMAGES="images"

mkdir -p $IMAGES

mv image-builder/deploy/debian-*/$NAME-$SIZE.img.zst $IMAGES

# generate sha256
sha256sum $IMAGES/$NAME-$SIZE.img.zst > $IMAGES/$NAME-$SIZE.img.zst.sha256
