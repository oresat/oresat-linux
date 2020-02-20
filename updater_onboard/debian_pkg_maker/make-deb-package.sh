#!/bin/bash

PKG_NAME="oresat-linux-updater"

# remove old build dir, if it exists
TEMP=$PKG_NAME"*"
rm -rf $TEMP

VERSION=`grep -i version deb-pkg-files/control | cut -d ":" -f 2 | sed 's/ //g'`
ARCHITECTURE=`grep -i architecture deb-pkg-files/control | cut -d ":" -f 2 | sed 's/ //g'`

BUILD_DIR=$PKG_NAME"_"$VERSION"_"$ARCHITECTURE

# Create the build dir
mkdir $BUILD_DIR

# Create the DEBIAN dir
DEBIAN_DIR=$BUILD_DIR"/DEBIAN/"
mkdir $DEBIAN_DIR
cp ./deb-pkg-files/* $DEBIAN_DIR

# create debian binary
echo "2.0" > $BUILD_DIR"/debian-binary"

# systemd dbus
DBUS_PATH=$BUILD_DIR"/usr/share/dbus-1/system.d/"
mkdir -p $DBUS_PATH
cp ../org.oresat.linux.updater.conf $DBUS_PATH

# systemd daemon service file
DAEMON_PATH=$BUILD_DIR"/lib/systemd/system/"
mkdir -p $DAEMON_PATH
cp ./oresat-linux-updaterd.service $DAEMON_PATH

# source code
SOURCE_PATH=$BUILD_DIR"/usr/share/"$PKG_NAME
mkdir -p $SOURCE_PATH
cp ../src/*.py $SOURCE_PATH

#make md5sums file
cd $BUILD_DIR
find . -type f ! -regex '.*?debian-binary.*' ! -regex '.*?DEBIAN.*' -printf '%P ' | xargs md5sum > DEBIAN/md5sums
cd -

dpkg -b $BUILD_DIR"/"
