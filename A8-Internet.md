### Routing Internet to Beaglebone from linux host laptop
On the Beaglebone terminal
- `sudo route add default gw 192.168.7.1`
- `sudo echo "nameserver 8.8.8.8" >> /etc/resolv.conf`

In the linux host computer terminal
- replace <WIRLESS> with wireless info from ip link (something like wlp3s0)
- replace <ETHERNET> with wireless info from ip link (something like enp0s26f7u1u3)
- `sudo sysctl net.ipv4.ip_forward=1`
- `iptables --table nat --append POSTROUTING --out-interface <WIRELESS> -j MASQUERADE`
- `iptables --append FORWARD --in-interface <ETHERNET> -j ACCEPT`
