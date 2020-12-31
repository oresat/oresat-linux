#!/usr/bin/python3

import logging
from ptftplib.tftpserver import TFTPServer
from ptftplib import tftpserver
from bootpserver import BOOTPServer, BootpPacket, NotBootpPacketError
from time import sleep
import os
import netifaces
import sys
import threading

log = logging.getLogger("octavo-usb-boot")
handler = logging.StreamHandler(stream=sys.stdout)
handler.setFormatter(logging.Formatter('%(levelname)s(%(name)s): %(message)s'))
log.addHandler(handler)
log.setLevel(logging.INFO)

iface = ""
root = ""

def wait_interface():
    # wait for usb0 appear, or timeout
    attempts = 0
    while True:
            intfs = netifaces.interfaces()
            if iface not in intfs:
                    attempts += 1
                    if attempts > 20:
                            raise Exception("{0} was not available".format(iface) )
                    sleep(0.25)
            else:
                    break

    # set the ip addr for usb0. I wish there was a more elegant way to do this
    os.system("/usr/sbin/ifconfig usb0 192.168.0.1 netmask 255.255.255.0")


def bootp_server():
	while True:
	    wait_interface()

	    # start the BOOTP/DHCP server
	    server = BOOTPServer(iface, "")

	    # serve requests until usb0 goes down
	    while True:
	    	# read from the socket. If it fails then break the loop so we can get a new server
                try:
                    data = server.sock.recv(4096)
                except OSError:
                    break

                # ignore non-BOOTP packets
                try:
                        pkt = BootpPacket(data)
                except NotBootpPacketError:
                        continue

                log.info("Handling '{0}' BOOTP packet".format(pkt.vendor_ident.decode('utf-8')))

                # return the proper filename based on the vendor identifier
                #
                # 1. 'AM335x ROM' is the vendor identifier for the first level AM335x ROM 
                # bootloader. Return the uboot SPL to this message
                if pkt.vendor_ident.decode() == "AM335x ROM":
                        server.bootfile = b'u-boot-spl-restore.bin'


                # 2. 'AM335x U-Boot SPL' is sent by the uboot SPL and in response we send
                # the actual uboot image name
                elif pkt.vendor_ident.decode() == "AM335x U-Boot SPL":
                        server.bootfile = b'u-boot-restore.img'
        
                # 3. For some reasons after getting the full uboot image it again needs the 
                # original SPL, and the vendor identifier sent for this is 'U-Boot/armv7'.
                # I don't know why this is, but it doesn't work without it.
                elif pkt.vendor_ident.decode() == "U-Boot.armv7":
                        server.bootfile = b'u-boot-spl-restore.bin'
                # error on unrecognized identifiers
                else:
                        raise Exception("Unknown vendor identifier: ", pkt.vendor_ident)

                # send the packet response
                server.handle_bootp_request(pkt)

def tftp_server():
    wait_interface()
    sleep(0.1)
    sys.argv = ['tftpserver', '-v', '-r', iface, root]
    tftpserver.main()
    #server = TFTPServer(iface, root, 69, True )
    #server.serve_forever()

def main():
    # get the interface and directory
    global iface
    global root
    iface = sys.argv[1]
    root = sys.argv[2]

    # start the ftrp server first and wait for it to start
    t = threading.Thread(target=tftp_server)
    t.start()
    sleep(1)

    # start the bootp server
    bootp_server()

if __name__ == '__main__':
    try:
        sys.exit(main())
    except KeyboardInterrupt:
        pass

