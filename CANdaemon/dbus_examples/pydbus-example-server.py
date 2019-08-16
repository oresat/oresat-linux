#!/usr/bin/env python3

from pydbus.generic import signal
from pydbus import SessionBus
from gi.repository import GLib
import random

class Server_XML(object):
    dbus = """
    <node>
        <interface name="org.example.project.oresat">
            <method name='Command'>
                <arg type='s' name='command' direction='in'/>
                <arg type='s' name='response' direction='out'/>
            </method>
            <method name='Quit'/>
            <signal name="data_signal">
                <arg type='s'/>
                <arg type='d'/>
                <arg type='i'/>
            </signal>
            <signal name="file_signal">
                <arg type='s'/>
            </signal>
        </interface>
    </node>
    """
    data_signal = signal()
    file_signal = signal()

    def Command(self, s):
        """to pass command from C3 card"""
        print(s)
        return "done"

    def Quit(self):
        """removes this object from the DBUS connection and exits"""
        loop.quit()

def send_data():
    """Generate random integer between 0 and 100 and emit over Session D-Bus
    return True to keep the GLib timer calling this function once a second."""
    test_string = "HelloWorld"
    random_double = random.uniform(0.0,10.0)
    random_integer = random.randint(0,100)
    print(test_string, random_double, random_integer)
    emit.data_signal(test_string, random_double, random_integer)
    return True

def send_file_path():
    emit.file_signal("/home/debian/test_file.tar.gz")
    print("/home/debian/test_file.tar.gz")
    return True

if __name__=="__main__":
    loop = GLib.MainLoop()
    interface_name = "org.example.project.oresat"
    bus = SessionBus()

    # Setup server to emit signals over the DBus
    emit = Server_XML()
    bus.publish(interface_name, emit)

    # Setup repeating timer
    GLib.timeout_add_seconds(interval=1, function=send_data) 
    GLib.timeout_add_seconds(interval=5, function=send_file_path) 

    # Run loop with graceful ctrl C exiting.
    try:
        loop.run()
    except KeyboardInterrupt as e:
        loop.quit()
        print("\nExit by Control C")
