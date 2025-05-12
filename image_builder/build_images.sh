#!/bin/bash -e

IMAGE_BUILDER_ROOT=$(pwd)
#IMAGE_BUILDER_ROOT=../../../..

build_board_deployment () {
    cd image-builder
    ./RootStock-NG.sh -c $BOARD
    mkdir $BOARD
    mv deploy $BOARD
    cd ..
}

compress_image () {
    # compress
    zstd $NAME-$SIZE.img

    cd $IMAGE_BUILDER_ROOT

    mv image-builder/$BOARD/deploy/debian-*/$NAME-$SIZE.img.zst $IMAGE_DIR

    # generate sha256
    sha256sum $IMAGE_DIR/$NAME-$SIZE.img.zst > $IMAGE_DIR/$NAME-$SIZE.img.zst.sha256
}

create_main_image () {
    local SIZE="4gb"
    if [ $BOARD == "oresat-dev" ]; then
        local DTB="oresat-dev-bootloader"
    else
        local DTB="oresat-bootloader"
    fi

    cd image-builder/$BOARD/deploy/debian-*/
    
    echo "$(pwd)"
    echo "$(ls -l)"
    
    cp $IMAGE_BUILDER_ROOT/$BOOTLOADER_DIR/{$SPL,$BOOTLOADER} ./u-boot

    # make .img file
    cp $IMAGE_BUILDER_ROOT/configs/$DTB.conf ./hwpack
    cp $IMAGE_BUILDER_ROOT/image-builder/tools/setup_sdcard.sh .
    sudo ./setup_sdcard.sh --img-$SIZE $NAME.img --dtb $DTB $SETUP_SDCARD_EXTRA_ARGS > log.txt

    compress_image
}

create_update_image () {
    local SIZE="2gb"
    local DTB="oresat-nobootloader"

    cd image-builder/$BOARD/deploy/debian-*/

    # make .img file
    cp $IMAGE_BUILDER_ROOT/configs/$DTB.conf ./hwpack
    cp $IMAGE_BUILDER_ROOT/image-builder/tools/setup_sdcard.sh .
    sudo ./setup_sdcard.sh --img-$SIZE $NAME.img --dtb $DTB > log2.txt

    compress_image
}

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
BOOTLOADER_DIR="u-boot"
SPL="MLO"
BOOTLOADER="u-boot-dtb.img"
IMAGE_DIR="images"

SETUP_SDCARD_EXTRA_ARGS=" \
    --spl $BOOTLOADER_DIR/$SPL \
    --bootloader $BOOTLOADER_DIR/$BOOTLOADER \
"

if [ $BOARD != "oresat-dev" ] && [ $BOARD != "oresat-generic" ]; then
    SETUP_SDCARD_EXTRA_ARGS="--enable-uboot-disable-pru ${SETUP_SDCARD_EXTRA_ARGS}"
fi

echo "setup_sdcard.sh options: $SETUP_SDCARD_EXTRA_ARGS"

mkdir -p $IMAGE_DIR

# copy oresat config into correct dirs for RootStock-NG.sh
cp ./configs/$BOARD.conf ./image-builder/configs/
cp ./chroot_scripts/*.sh ./image-builder/target/chroot/

# clear any previous builds
#rm -rf deploy

# build partitions
if [ ! -d "image-builder/$BOARD" ]; then
    build_board_deployment
fi

echo "$(pwd)"
create_main_image

if [ $BOARD != "oresat-dev" ]; then
    create_update_image
fi

