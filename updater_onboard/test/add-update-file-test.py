#!/usr/bin/env python3


from pydbus.generic import signal
from pydbus import SystemBus
from gi.repository import GLib


INTERFACE_NAME = "org.OreSat.Updater"
EXAMPLE_UPDATE_DIR = "/home/debian/oresat-linux/updater_onboard/test/example_updates/"


if __name__=="__main__":
    bus = SystemBus() # connect to bus
    loop = GLib.MainLoop() # only used by server
    the_object = bus.get(INTERFACE_NAME)

    update_file = EXAMPLE_UPDATE_DIR + "update-2020-01-03-08-57.tar.gz"

    print("Calling AddUpdateFile method with {}".format(update_file))
    reply = the_object.AddUpdateFile(update_file)
    print("Returned: {}\n".format(reply))
