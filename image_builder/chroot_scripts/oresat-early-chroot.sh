#!/bin/sh -e

TEMPDIR=`grep -i tempdir= .project | cut -d "=" -f 2 | sed 's/\"//g'`
HOSTNAME=`grep -i hostname= .project | cut -d "=" -f 2 | sed 's/\"//g'`

# to remove install installing recommended and suggestes packages
# note image-builder removes this file during cleanup
cat > "${TEMPDIR}/etc/apt/apt.conf" <<-__EOF__
APT::Install-Suggests "0";
APT::Install-Recommends "0";
__EOF__

# add this to use piwheels.org as an extra source
# piwheels.org has pre-built "manylinux" armhf packages
# otherwise packages like numpy, psutil, and opencv-python have to be build from pypi source
cat > "${TEMPDIR}/etc/pip.conf" <<-__EOF__
[global]
extra_index_url = https://piwheels.org/simple
__EOF__

# add all oresat device trees to /tmp
cp ../../device_trees/*.dtb $TEMPDIR/tmp

# add custom scripts
cp -r ../scripts $TEMPDIR/opt

# add custom deb packages
if [ "$HOSTNAME" = "oresat-c3" ]; then
    cp ../packages/ax5043_* $TEMPDIR/tmp
    cp ../packages/oresat-c3-watchdog_* $TEMPDIR/tmp
elif [ "$HOSTNAME" = "oresat-cfc" ]; then
    cp ../packages/prucam-pirt1280-dkms_* $TEMPDIR/tmp
elif [ "$HOSTNAME" = "oresat-star-tracker" ]; then
    cp ../packages/prucam-ar013x-dkms_* $TEMPDIR/tmp
elif [ "$HOSTNAME" = "oresat-dxwifi" ]; then
    cp ../packages/oresat-dxwifi-* $TEMPDIR/tmp
fi
