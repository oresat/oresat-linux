Connecting to OreSat Linux from Linux host
==========================================

.. note:: This guide only goes over Linux systems using `NetworkManager`_ or
   `systemd-networkd`_

- Connect the OreSat Linux board to host using a USB cable
- In a terminal run the following command and wait for the two connection from
  the OreSat Linux board to come up.

  .. code-block:: text

    $ watch -n 1 ip a
    1: lo: <LOOPBACK,UP,LOWER_UP> mtu 65536 qdisc noqueue state UNKNOWN group default qlen 1000
      ...
    2: wlo1: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc noqueue state UP group default qlen 1000
      ...
    3: enp0s20f0u3: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc fq_codel state UNKNOWN group default qlen 1000
        link/ether 60:64:05:f9:0d:06 brd ff:ff:ff:ff:ff:ff
    4: enp0s20f0u3i2: <NO-CARRIER,BROADCAST,MULTICAST,UP> mtu 1500 qdisc fq_codel state DOWN group default qlen 1000
        link/ether 60:64:05:f9:0d:08 brd ff:ff:ff:ff:ff:ff


- Make a note of the second connection name. If th is a ``enp*`` connection it
  is usually the ``enp*i2`` one (as show above) or if it is a ``enx*``
  connection it is the one with the higher number at the end of the name.

- **Only if the Linux host uses** `NetworkManager`_

  - Install ``dnsmasq`` on your system

  - In another terminal setup a DHCP server. **Replace** ``enp0s20f0u3i2`` in the follow
    command with the name found in previous step on your host.

    .. note:: This guide use the `nmcli` command as the NetworkManager applet is limited

    .. code-block:: text

      $ nmcli connection add type ethernet ifname enp0s20f0u3i2 ipv4.method shared con-name oresat-board

- **Only if the Linux host uses** `systemd-networkd`_

  - Setup the DHCP server. With your favorite IDE or text editor make the
    ``/etc/systemd/network/20-oresat-board.network`` file and add the following.
    **Replace** enp*i2 as needed.

      .. code-block:: text

        [Match]
        Name=enp*i2

        [Network]
        Address=10.42.1.1/24
        DHCPServer=true
        IPMasquerade=ipv4
        MulticastDNS=yes

        [DHCPServer]
        EmitDNS=yes
        DNS=9.9.9.9

  - Restart systemd-networkd

    .. code-block:: text

      $ sudo systemctl restart systemd-networkd

- Go back to the terminal with ``watch ip a`` command running at wait for the
  address to be set. Make a note of the address and IPv4 address space, it
  should be something like ``10.42.1.1/24``. 

  .. code-block:: text

    $ watch -n 1 ip a
    1: lo: <LOOPBACK,UP,LOWER_UP> mtu 65536 qdisc noqueue state UNKNOWN group default qlen 1000
      ...
    2: wlo1: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc noqueue state UP group default qlen 1000
      ...
    3: enp0s20f0u3: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc fq_codel state UNKNOWN group default qlen 1000
        link/ether 60:64:05:f9:0d:06 brd ff:ff:ff:ff:ff:ff
    4: enp0s20f0u3i2: <NO-CARRIER,BROADCAST,MULTICAST,UP> mtu 1500 qdisc fq_codel state DOWN group default qlen 1000
        link/ether 60:64:05:f9:0d:08 brd ff:ff:ff:ff:ff:ff
        inet 10.42.1.1/24 scope host lo
          valid_lft forever preferred_lft forever
        inet6 fe80::cbcb:674d:2adf/64 scope link noprefixroute
          valid_lft forever preferred_lft forever

- Install ``nmap`` for your system

- Run nmap to figure out the board's IP address. **Replace** ``10.42.1.1/24`` in
  the follow command to be addres **with** the IPv4 address space found in the
  previous step.

  .. code-block:: text

    $ nmap -sn 10.42.1.1/24
    Starting Nmap 7.92 ( https://nmap.org ) at 2022-04-13 22:07 UTC
    Nmap scan report for 10.42.1.1
    Host is up (0.00079s latency).
    Nmap scan report for 10.42.1.120
    Host is up (0.0035s latency).
    Nmap done: 256 IP addresses (4 hosts up) scanned in 2.63 seconds

- That should print out two IP addresses (one is the Linux host and one is the
  OreSat board)

- The terminal with ``watch ip a`` running is no longer needed.

- SSH onto the board. Password is ``temppwd``. **Replace** ``10.42.1.120`` in
  the following command with other address that nmap found

  .. code-block:: text

    $ ssh debian@10.42.1.120

- Make sure shared internet is working

  .. code-block:: text

    $ ping www.google.com
    PING www.google.com (172.217.14.228) 56(84) bytes of data.
    64 bytes from sea30s02-in-f4.1e100.net (172.217.14.228): icmp_seq=1 ttl=117 time=41.6 ms
    64 bytes from sea30s02-in-f4.1e100.net (172.217.14.228): icmp_seq=2 ttl=117 time=29.1 ms

.. _systemd-networkd: https://wiki.archlinux.org/index.php/Systemd-networkd
.. _NetworkManager: https://networkmanager.dev/
