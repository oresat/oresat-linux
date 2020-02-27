#!/usr/bin/env python3

from pydbus import SystemBus
from gi.repository import GLib
import sys


DESTINATION = "org.OreSat.Example"
#INTERFACE_NAME = "org.OreSat.Example"
INTERFACE_NAME = "org.freedesktop.DBus.Properties"
OBJECT_PATH = "/org/OreSat/Example"


def cb_server_signal_emission(*args):
    """
    Callback on emitting signal from server
    """
    print("Data: ", args[4])


def main():
    bus = SystemBus() # connect to bus
    loop = GLib.MainLoop()

    bus.subscribe(sender=DESTINATION, iface=INTERFACE_NAME, signal="PropertiesChanged", object=OBJECT_PATH, arg0=None, flags=0, signal_fired=cb_server_signal_emission)

    # Run loop with graceful ctrl C exiting.
    try:
        loop.run()
    except KeyboardInterrupt as e:
        loop.quit()

if __name__=="__main__":
    main()
