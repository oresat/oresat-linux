#!/bin/bash -e
# this script is run in the linux host context, not the target context
# specifically, it hooks into the build process right before the omap-chroot script

# load path to target from arguments
tempdir=$1

# to remove install installing recommended and suggestes packages
# note image-builder removes this file during cleanup
cat <<__EOF__ >"${tempdir}/etc/apt/apt.conf"
APT::Install-Suggests "0";
APT::Install-Recommends "0";
__EOF__

# add oresat debian repo
echo "deb [trusted=yes] https://packages.oresat.org/debian/ ./" >>"${tempdir}/etc/apt/sources.list"

# add this to use piwheels.org as an extra source
# piwheels.org has pre-built "manylinux" armhf packages
# otherwise packages like numpy, psutil, and opencv-python have to be build from pypi source
cat <<__EOF__ >"${tempdir}/etc/pip.conf"
[global]
extra_index_url = https://piwheels.org/simple
__EOF__

# add all oresat device trees to /tmp
cp -v ../../device_trees/*.dtb "${tempdir}/tmp"

# add custom scripts
cp -vr ../scripts "${tempdir}/opt"
