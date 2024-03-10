#!/bin/sh -e

tempdir=`grep -i tempdir= .project | cut -d "=" -f 2 | sed 's/\"//g'`

# to remove install installing recommended and suggestes packages
# note image-builder removes this file during cleanup
cat > "${tempdir}/etc/apt/apt.conf" <<-__EOF__
APT::Install-Suggests "0";
APT::Install-Recommends "0";
__EOF__

# add this to use piwheels.org as an extra source
# piwheels.org has pre-built "manylinux" armhf packages
# otherwise packages like numpy, psutil, and opencv-python have to be build from pypi source
cat > "${tempdir}/etc/pip.conf" <<-__EOF__
[global]
extra_index_url = https://piwheels.org/simple
__EOF__

# add all oresat device trees to /tmp
cp ../../device_trees/*.dtb $tempdir/tmp

# add custom scripts
cp -r ../scripts $tempdir/opt
