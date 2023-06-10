import logging
import os
import sys
import threading
from time import sleep
from argparse import ArgumentParser

import psutil
from ptftplib.tftpserver import TFTPServer
from ptftplib import notify

from . import IP_ADDR
from .bootpserver import BootpPacket, BOOTPServer, NotBootpPacketError

LOGGER_FMT = '%(levelname)s(%(name)s): %(message)s'

log = logging.getLogger("octavo-usb-boot")
handler = logging.StreamHandler(stream=sys.stdout)
handler.setFormatter(logging.Formatter(LOGGER_FMT))
log.addHandler(handler)
log.setLevel(logging.INFO)


def wait_interface(iface: str):
    # wait for usb0 appear, forever
    while True:
        if iface in psutil.net_if_stats():
            break

    # set the ip addr for usb0. I wish there was a more elegant way to do this
    os.system(f"/usr/sbin/ifconfig {iface} {IP_ADDR} netmask 255.255.255.0")


def bootp_server(iface: str):
    while True:
        # wait for the interface to come up
        wait_interface(iface)

        # wait a sec for the server to start
        sleep(2)

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


def tftp_server(iface: str, root: str):

    # make sure root is absolute path
    root = os.path.abspath(root)

    wait_interface(iface)

    sleep(0.1)

    # enable verbose logging
    logger = notify.getLogger('tftpd')
    notify.StreamEngine.install(
        logger,
        stream=sys.stdout,
        loglevel=logging.INFO,
        fmt=LOGGER_FMT
    )

    server = TFTPServer(iface, root, strict_rfc1350=True)

    try:
        server.serve_forever()
    except Exception as e:
        log.info(e)


if __name__ == '__main__':
    paser = ArgumentParser(description='flash an Octavo A8 eMMC over USB')
    paser.add_argument('iface', help='the network interface to use')
    paser.add_argument('root', help='path to tftp data directory')
    args = paser.parse_args()

    t = threading.Thread(target=tftp_server, args=(args.iface, args.root))
    t.start()
    sleep(1)

    try:
        bootp_server(args.iface)
    except KeyboardInterrupt:
        pass
