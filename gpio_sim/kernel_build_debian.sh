#!/bin/bash

# For Debian 
# Build and install GPIO-SIM kernel module
# Main reference https://www.debian.org/doc/manuals/debian-kernel-handbook/ch-common-tasks.html

# Install prerequisites
sudo apt-get update
sudo apt-get -y install dpkg-dev
sudo apt-get -y install build-essential fakeroot
sudo apt-get -y build-dep linux

# Get linux source
sudo apt-get -y install linux-source

# Get the directory of the sourced script
SCRIPT_DIR=$(dirname "$(realpath "$0")")

# Make the build directory
BUILD_DIR=$SCRIPT_DIR/Kernel_Build
mkdir -p $BUILD_DIR

cd /usr/src
# Find the latest version of the downloaded source
SOURCE=$(ls | grep linux-source | sort -V | tail -n 1)

# Extract the source
tar xaf /usr/src/$SOURCE -C $BUILD_DIR

# Get the version number (X.Y) from the source name
VERSION=${SOURCE#linux-source-}
VERSION=${VERSION%.tar.bz2}
VERSION=${VERSION%.tar.xz}

# Move into the source directory in the build directory
cd $BUILD_DIR/linux-source-$VERSION

# Build the Kernel
MAKEFLAGS="-j$(nproc) LOCALVERSION=-gpio-sim CONFIG_GPIO_SIM=y"
make clean
make oldconfig # Combine this Kernel's configuration with the current running Kernel
make deb-pkg $MAKEFLAGS

# Move to the parent directory where the .deb files are located
cd ..

# Confirm before installing the kernel packages
read -p "Are you sure you want to install the kernel packages? (y/n) " -n 1 -r
echo    # (optional) move to a new line
if [[ $REPLY =~ ^[Yy]$ ]]
then
    sudo dpkg -i linux-image*.deb linux-headers*.deb
    echo "Restart your machine to have the new build-in modules load on boot"
fi
