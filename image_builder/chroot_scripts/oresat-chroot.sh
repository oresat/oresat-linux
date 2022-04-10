#!/bin/sh -e

##############################################################################
echo "disable root login"

echo "PermitRootLogin prohibit-password" >> /etc/ssh/sshd_config

##############################################################################
echo "set default boot power mode"

HOSTNAME=`cat /etc/hostname`
if [ $HOSTNAME != "oresat-dev" ]; then
cat > "/etc/default/cpufrequtils" <<-__EOF__
GOVERNOR="powersave"
__EOF__
else
cat > "/etc/default/cpufrequtils" <<-__EOF__
GOVERNOR="performance"
__EOF__
fi

##############################################################################
echo "remove internet packages required during build on flight images"

if [ $HOSTNAME != "oresat-dev" ]; then
apt -y purge git git-man curl wget rsync
fi
