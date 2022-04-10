#!/bin/sh -e

# to remove install installing recommended and suggestes packages
cat > "${tempdir}/etc/apt/apt.conf" <<-__EOF__
APT::Install-Suggests "0";
APT::Install-Recommends "0";
__EOF__

# add oresat debian repo
#echo "deb [trusted=yes] https://packages.oresat.org/debian bullseye main" >> ${tempdir}/etc/apt/sources.list
#apt update
