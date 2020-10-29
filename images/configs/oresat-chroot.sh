#!/bin/sh -e
#
# Copyright (c) 2014-2020 Robert Nelson <robertcnelson@gmail.com>
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.

export LC_ALL=C

#contains: rfs_username, release_date
if [ -f /etc/rcn-ee.conf ] ; then
	. /etc/rcn-ee.conf
fi

if [ -f /etc/oib.project ] ; then
	. /etc/oib.project
fi

export HOME=/home/${rfs_username}
export USER=${rfs_username}
export USERNAME=${rfs_username}

echo "env: [`env`]"

is_this_qemu () {
	unset warn_qemu_will_fail
	if [ -f /usr/bin/qemu-arm-static ] ; then
		warn_qemu_will_fail=1
	fi
}

qemu_warning () {
	if [ "${warn_qemu_will_fail}" ] ; then
		echo "Log: (chroot) Warning, qemu can fail here... (run on real armv7l hardware for production images)"
		echo "Log: (chroot): [${qemu_command}]"
	fi
}

git_clone () {
	mkdir -p ${git_target_dir} || true
	qemu_command="git clone ${git_repo} ${git_target_dir} --depth 1 || true"
	qemu_warning
	git clone ${git_repo} ${git_target_dir} --depth 1 || true
	chown -R 1000:1000 ${git_target_dir}
	sync
	echo "${git_target_dir} : ${git_repo}" >> /opt/source/list.txt
}

git_clone_branch () {
	mkdir -p ${git_target_dir} || true
	qemu_command="git clone -b ${git_branch} ${git_repo} ${git_target_dir} --depth 1 || true"
	qemu_warning
	git clone -b ${git_branch} ${git_repo} ${git_target_dir} --depth 1 || true
	chown -R 1000:1000 ${git_target_dir}
	sync
	echo "${git_target_dir} : ${git_repo}" >> /opt/source/list.txt
}

git_clone_full () {
	mkdir -p ${git_target_dir} || true
	qemu_command="git clone ${git_repo} ${git_target_dir} || true"
	qemu_warning
	git clone ${git_repo} ${git_target_dir} || true
	chown -R 1000:1000 ${git_target_dir}
	sync
	echo "${git_target_dir} : ${git_repo}" >> /opt/source/list.txt
}

##############################################################################

is_this_qemu

echo "" >> /etc/securetty
echo "#USB Gadget Serial Port" >> /etc/securetty
echo "ttyGS0" >> /etc/securetty

##############################################################################
# set default boot power mode

cat > "/etc/default/cpufrequtils" <<-__EOF__
GOVERNOR="powersave"
__EOF__

##############################################################################
# setup usb ethernet

#cat > "/etc/default/bb-boot" <<-__EOF__
#USB_CONFIGURATION=enable
#USB_NETWORK_DISABLED="yes"
#__EOF__

#echo "g_ether" > /etc/modules-load.d/g_ether.conf
#HOST_ADDR=`dmesg | grep "usb0: HOST MAC" | cut -d " " -f 8`
#echo "options g_ether host_addr=$HOST_ADDR" > /etc/modprobe.d/g_ether.conf

##############################################################################
# setup systemd-networkd

cat > "/etc/systemd/network/10-can.network" <<-__EOF__
[Match]
Name=can1

[CAN]
BitRate=1M
__EOF__


cat > "/etc/systemd/network/20-wired.network" <<-__EOF__
[Match]
Name=usb0

[Network]
Address=168.192.6.2/24
Gateway=168.192.6.1
DNS=168.192.6.1
DNS=8.8.8.8
__EOF__

# enable systemd-networkd on boot
systemctl enable systemd-networkd
systemctl enable systemd-resolved

##############################################################################
# remove internet things

apt -y remove git curl wget rsync ca-certificates
