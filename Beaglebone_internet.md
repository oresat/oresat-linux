# Beaglebone internet routing notes
By default, Beaglebones has two static IP addresses: 192.168.7.2 (ment for host windows computers) and 192.168.6.2 (ment for host Linux or MacOS computers). It expects the host computer to be 192.168.7.1 or 192.168.6.1 respectively.

## Linux host computer using iptables
- In the Beaglebone terminal
    - `sudo route add default gw 192.168.7.1` or `sudo route add default gw 192.168.6.1`
    - `sudo echo "nameserver 8.8.8.8" >> /etc/resolv.conf`
- In the host Linux computer terminal
    - replace <INTERNET_INTERFACE> with info from ip link (something like wlp3s0 or enp3s0)
    - replace <BBB_INTERFACE> with info from ip link (something like enp0s26f7u1u3)
    - `sudo sysctl net.ipv4.ip_forward=1`
    - `sudo iptables --table nat --append POSTROUTING --out-interface <INTERNET_INTERFACE> -j MASQUERADE`
    - `sudo iptables --append FORWARD --in-interface <BBB_INTERFACE> -j ACCEPT`

## Mac host computer
[Guide from Solarian Programmer](https://solarianprogrammer.com/2018/12/04/beaglebone-black-sharing-usb-internet-macos/)

## Windows host computer
[Guide from digikey](https://www.digikey.com/en/maker/blogs/how-to-connect-a-beaglebone-black-to-the-internet-using-usb)
