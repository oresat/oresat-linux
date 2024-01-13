Connecting to OreSat Linux from MacOS host
==========================================

- Connect the OreSat Linux card to host using a USB cable

- Go to ``System Preference => Network`` and wait for the two connections to come
  up
- Remove the ``BeagleBone`` connection (the first connection), we will only care
  about the ``BeagleBone 2`` connection

- Go to ``System Preference => Sharing``, click on ``Internet Sharing`` (the word,
  not the checkbox), check the checkbox next to ``BeagleBone``, and then check
  the checkbox for ``Internet Sharing``

- Open a terminal and watch ifconfig
  
  .. code-block::
    
    $ watch -n 1 ifconfig
    lo0: flags=8049<UP,LOOPBACK,RUNNING,MULTICAST> mtu 16384
      ...
    en0: flags=8822<BROADCAST,SMART,SIMPLEX,MULTICAST> mtu 1500
      ...
    en7: flags=8963<UP,BROADCAST,SMART,RUNNING,PROMISC,SIMPLEX,MULTICAST> mtu 1486
      ether 60:64:05:f9:0d:06 
      nd6 options=201<PERFORMNUD,DAD>
      media: autoselect
      status: active
    bridge100: flags=8863<UP,BROADCAST,SMART,RUNNING,SIMPLEX,MULTICAST> mtu 1486
      options=3<RXCSUM,TXCSUM>
      ether ca:2a:14:44:1d:64 
      inet 192.168.2.1 netmask 0xffffff00 broadcast 192.168.2.255
      inet6 fe80::c82a:14ff:fe44:1d64%bridge100 prefixlen 64 scopeid 0xe 
      Configuration:
        id 0:0:0:0:0:0 priority 0 hellotime 0 fwddelay 0
        maxage 0 holdcnt 0 proto stp maxaddr 100 timeout 1200
        root id 0:0:0:0:0:0 priority 0 ifcost 0 port 0
        ipfilter disabled flags 0x2
      member: en7 flags=3<LEARNING,DISCOVER>
              ifmaxaddr 0 port 13 priority 0 path cost 0
      nd6 options=201<PERFORMNUD,DAD>
      media: autoselect
      status: active
    en6: flags=8863<UP,BROADCAST,SMART,RUNNING,SIMPLEX,MULTICAST> mtu 1500
      ether 60:64:05:f9:0d:08 
      inet6 fe80::cf:3766:46e:b762%en6 prefixlen 64 secured scopeid 0xf 
      inet 169.254.58.83 netmask 0xffff0000 broadcast 169.254.255.255
      nd6 options=201<PERFORMNUD,DAD>
      media: autoselect (100baseTX <full-duplex>)
      status: active

- Once the bridge show up note the IP address and stop the watch (control-C).
  In the example ``ifconfig`` above, the address for the bridge would be
  ``192.168.2.1``.

- Go back to ``System Preference => Network`` and edit ``BeagleBone 2`` and
  change it to ``Using DHCP with manual address`` and set the address to the
  address found from ifconfig. 

- Wait for the ``Subnet Mask`` to be set

- Install ``nmap`` for your system
  
  - with brew::

      $ brew install nmap

  - **or** with MacPorts::

      $ sudo ports install nmap

  - **or** download and install from https://nmap.org/download.html/#macosx

- Run nmap to figure out the card's IP address. **Replace** ``192.168.2.1/24`` in
  the follow command to be address **with** the IPv4 address space found by
  ifconfig.

  .. code-block:: text

    $ nmap -sn 192.168.2.1/24
    Starting Nmap 7.92 ( https://nmap.org ) at 2022-04-13 22:07 UTC
    Nmap scan report for 192.168.2.1
    Host is up (0.00079s latency).
    Nmap scan report for 192.168.2.6
    Host is up (0.0035s latency).
    Nmap done: 256 IP addresses (4 hosts up) scanned in 2.63 seconds

- That should print out two IP addresses (one is the MacOS host and one is the
  OreSat card)

- SSH onto the card. Password is ``temppwd``. **Replace** ``192.168.2.6`` in the
  following command with other address that nmap found.

  .. code-block:: text

    $ ssh debian@192.168.2.6

- Make sure shared internet is working

  .. code-block:: text

    $ ping www.google.com
    PING www.google.com (172.217.14.228) 56(84) bytes of data.
    64 bytes from sea30s02-in-f4.1e100.net (172.217.14.228): icmp_seq=1 ttl=117 time=41.6 ms
    64 bytes from sea30s02-in-f4.1e100.net (172.217.14.228): icmp_seq=2 ttl=117 time=29.1 ms
