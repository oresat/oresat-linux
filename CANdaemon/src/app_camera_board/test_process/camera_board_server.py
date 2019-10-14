#!/usr/bin/env python3

'''
Test server for Camera Board
'''

from pydbus.generic import signal
from pydbus import SystemBus
from gi.repository import GLib
import random

class Server_XML(object):
    dbus = """
    <node>
        <interface name="org.OreSat.CameraBoard">
            <method name='Quit'/>
            <signal name="OrientationSignal">
                <arg type='n'/>
                <arg type='n'/>
                <arg type='n'/>
            </signal>
            <signal name="NewImageSignal">
                <arg type='s'/>
            </signal>
        </interface>
    </node>
    """
    OrientationSignal = signal()
    NewImageSignal = signal()

    def Quit(self):
        """removes this object from the DBUS connection and exits"""
        loop.quit()

def send_file_path():
    filepath = "./test_process/psas.jpg" #TODO should be an absoult path
    emit.NewImageSignal(filepath)
    print(filepath)
    return True

if __name__=="__main__":
    loop = GLib.MainLoop()
    interface_name = "org.OreSat.CameraBoard"
    bus = SystemBus()

    # Setup server to emit signals over the DBus
    emit = Server_XML()
    bus.publish(interface_name, emit)

    # Setup repeating timer
    GLib.timeout_add_seconds(interval=5, function=send_file_path) 

    try:
        loop.run()
    except KeyboardInterrupt as e:
        loop.quit()