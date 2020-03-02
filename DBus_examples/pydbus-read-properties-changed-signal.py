#!/usr/bin/env python3


from pydbus import SystemBus
from gi.repository import GLib


def data_cb(*args):
    """
    Callback on emitting signal from server
    """
    print("Data Signal: ", args[4])


if __name__=="__main__":
    bus = SystemBus() # connect to bus
    loop = GLib.MainLoop()

    bus.subscribe(
            sender="org.OreSat.Example",
            iface="org.freedesktop.DBus.Properties",
            signal_fired=data_cb,
            object="/org/OreSat/Example")

    try:
        loop.run()
    except KeyboardInterrupt as e:
        loop.quit()
