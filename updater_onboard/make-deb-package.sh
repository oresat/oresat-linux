#!/bin/bash

PKG_NAME="oresat-linux-updater"

# remove old build dir, if it exist
rm -rf $PKG_NAME

# Create the build dir
mkdir $PKG_NAME

# Create the DEBIAN dir
DEBIAN_DIR=$PKG_NAME"/DEBIAN/"
mkdir $DEBIAN_DIR
cp deb-pkg-files/* $DEBIAN_DIR

# create debian binary
echo "2.0" > $PKG_NAME"/debian-binary"

# systemd dbus 
DBUS_PATH=$PKG_NAME"/usr/share/dbus-1/system.d/"
mkdir -p $DBUS_PATH
cp org.oresat.linux.updater.conf $DBUS_PATH

# systemd daemon service file
DAEMON_PATH=$PKG_NAME"/usr/lib/systemd/system/"
mkdir -p $DAEMON_PATH
cp oresat-linux-updater.service $DAEMON_PATH

# source code
SOURCE_PATH=$PKG_NAME"/usr/bin/"$PKG_NAME
mkdir -p $SOURCE_PATH
cp updater.py $SOURCE_PATH
cp updater_daemon.py $SOURCE_PATH

#make md5sums file
cd $PKG_NAME
find . -type f ! -regex '.*?debian-binary.*' ! -regex '.*?DEBIAN.*' -printf '%P ' | xargs md5sum > DEBIAN/md5sums
cd -

dpkg -b $PKG_NAME"/"
