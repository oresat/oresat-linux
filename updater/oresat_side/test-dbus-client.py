#!/usr/bin/env python3


from pydbus import SystemBus
from gi.repository import GLib
import sys


INTERFACE_NAME = "org.OreSat.Updater"

def cb_server_signal_emission(*args):
    """
    Callback on emitting signal from server
    """
    print("Data: ", args)

if __name__=="__main__":
    bus = SystemBus()
    loop = GLib.MainLoop()

    a = bus.subscribe(iface=INTERFACE_NAME, )signal = "DataSignal")

    try:
        loop.run()
    except KeyboardInterrupt as e:
        loop.quit()


