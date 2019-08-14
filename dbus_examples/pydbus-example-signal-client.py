#!/usr/bin/env python3

from pydbus.generic import signal
from pydbus import SessionBus
from gi.repository import GLib

loop = GLib.MainLoop()
dbus_filter = "/org/example/project_1/server_1"
bus = SessionBus()


def cb_server_signal_emission(*args):
    """
    Callback on emitting signal from server
    """
    print("Data: ", args[4])

if __name__=="__main__":
    # Subscribe to bus to monitor for server signal emissions
    bus.subscribe(object = dbus_filter, 
                  signal_fired = cb_server_signal_emission)    
    loop.run()
