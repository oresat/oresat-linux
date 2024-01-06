#!/bin/bash -e

list="c3 cfc dev dxwifi gps star-tracker"

help_message() {
    echo "./build_images.sh <BOARD> <VERION>"
    echo ""
    echo "where <BOARD> can be:"
    for i in $list; do
        echo "  $i"
    done
    echo ""
    echo "where <VERION> is optional but must match a device tree version if "
    echo "added. If not added the latest version will be used. Not used on dev"
    echo "images."
    echo ""
    echo "Examples:"
    echo "  ./build_images.sh gps"
    echo "  ./build_images.sh c3 v1.0"
}

if [[ ! $list =~ (^|[[:space:]])$1($|[[:space:]]) ]]; then
    echo "Invalid board argument"
    echo ""
    help_message
    exit 1
fi

BOARD="oresat-"$1
DATE=`date "+%F"`
NAME="$BOARD-$DATE"
SIZE="2gb"
version=$2

if [ $BOARD == "oresat-dev" ]; then
    dtb=""
    version=""
elif [ -z "$version" ]; then
    dtss=(`ls device_trees/$BOARD-v*.dts`)
    dts=${dtss[-1]}
    dtb=device_trees/`basename -- "$dts" .dts`.dtb
    version=`echo $dtb | awk -F "-" '{print $NF}' | cut -d . -f 1 | sed 's/_/./g'`
else
    dtb=$BOARD-`echo $version | sed 's/./_/'`.dtb
    if [ ! -f device_trees/$dtb]; then
        echo "Invalid version argument"
        echo ""
        help_message
        exit 1
    fi
fi

if [ ! -d image-builder ]; then
    git clone https://github.com/beagleboard/image-builder
fi

# build all device trees
make -C device_trees

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
sudo ./setup_sdcard.sh --img-$SIZE $NAME.img --dtb beaglebone --enable-mainline-pru-rproc

# compress
zstd $NAME-$SIZE.img

cd ../../..

mv image-builder/deploy/debian-*/$NAME-$SIZE.img.zst .

# generate sha256
sha256sum $NAME-$SIZE.img.zst > $NAME-$SIZE.img.zst.sha256
