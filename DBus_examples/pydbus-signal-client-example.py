#!/usr/bin/env python3


from pydbus import SystemBus
from gi.repository import GLib


INTERFACE_NAME = "org.OreSat.Example"


def cb_server_signal_emission(*args):
    """
    Callback on emitting signal from server
    """
    print("Data: ", args[4])


if __name__=="__main__":
    bus = SystemBus() # connect to bus
    loop = GLib.MainLoop()

    # Subscribe to bus to monitor for all server signals emissions
    bus.subscribe(iface = INTERFACE_NAME,  signal_fired = cb_server_signal_emission)

    try:
        loop.run()
    except KeyboardInterrupt as e:
        loop.quit()
