#!/usr/bin/env python3


from pydbus.generic import signal
from pydbus import SystemBus
from gi.repository import GLib


INTERFACE_NAME = "org.OreSat.Updater"


if __name__=="__main__":
    bus = SystemBus() # connect to bus
    loop = GLib.MainLoop() # only used by server
    the_object = bus.get(INTERFACE_NAME)

    print("Calling StartUpdate\n")
    reply = the_object.StartUpdate()
    print("Returned: {}\n".format(reply))

