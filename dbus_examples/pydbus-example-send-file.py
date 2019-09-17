#!/usr/bin/env python3

import sys
from pydbus.generic import signal
from pydbus import SessionBus
from gi.repository import GLib
import random

loop = GLib.MainLoop()
filepath = ""

class Server_XML(object):
    dbus = """
    <node>
        <interface name="org.example.project.oresat">
            <signal name="file_signal">
                <arg type='s'/>
            </signal>
        </interface>
    </node>
    """
    file_signal = signal()

def send_file_path():
    emit.file_signal(filepath)
    print(filepath)
    loop.quit()


if __name__=="__main__":
    if(len(sys.argv) != 2):
        print("input error")
        print("python3 pydbus-example-send-file.py abs_file_path")
        exit(1)

    # get file abs filepath from args
    filepath = sys.argv[1]
    if(filepath[0] != '/'):
        print("uses absolute filepath (start from root)")
        exit(1)

    # set up dbus interface
    interface_name = "org.example.project.oresat"
    bus = SessionBus()

    # Setup server to emit signals over the DBus
    emit = Server_XML()
    bus.publish(interface_name, emit)

    GLib.timeout_add_seconds(interval=1, function=send_file_path) 
    loop.run()
