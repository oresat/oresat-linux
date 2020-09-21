# oresat generic after hook script


##############################################################################
# remove internet things

apt -y remove git curl wget rsync connman


##############################################################################
# set up one static usb

echo "g_ether" > /etc/modules-load.d/g_ether.conf
HOST_ADDR= `dmesg | grep "usb0: HOST MAC" | cut -d " " -f 8`
echo "options g_ether host_addr=$HOST_ADDR" > /etc/modprobe.d/g_ether.conf


##############################################################################
# setup systemd-networkd

cat > "etc/systemd/network/10-can.network" <<-__EOF__
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

sudo systemctl enable systemd-networkd
sudo systemctl enable systemd-resolved
