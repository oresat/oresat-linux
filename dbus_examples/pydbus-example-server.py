#!/usr/bin/env python3

from pydbus.generic import signal
from pydbus import SessionBus
from gi.repository import GLib
import random

class Server_XML(object):
    """
    Server_XML definition.
    Emit / Publish a signal that is a random integer every second 
    type='i' for integer. 
    """
    dbus = """
    <node>
        <interface name="org.example.project_1.server_1">
            <method name='Command'>
                <arg type='s' name='command' direction='in'/>
                <arg type='s' name='response' direction='out'/>
            </method>
            <method name='Quit'/>
            <signal name="app_1_signal">
                <arg type='s'/>
                <arg type='d'/>
            </signal>
        </interface>
    </node>
    """
    app_1_signal = signal()

    def Command(self, s):
        """to pass command from C3 card"""
        print(s)
        return "done"

    def Quit(self):
        """removes this object from the DBUS connection and exits"""
        loop.quit()

def repeating_timer():
    """Generate random integer between 0 and 100 and emit over Session D-Bus
    return True to keep the GLib timer calling this function once a second."""
    test_string = "HelloWorld"
    #random_integer = random.randint(0,100)
    random_double = random.uniform(0.0,10.0)
    print(test_string, random_double)
    #print(test_string, random_integer, random_double)
    #emit.app_1_signal(test_string, random_integer, random_double)
    emit.app_1_signal(test_string, random_double)
    return True

if __name__=="__main__":
    loop = GLib.MainLoop()
    interface_name = "org.example.project_1.server_1"
    bus = SessionBus()
    # Setup server to emit signals over the DBus
    emit = Server_XML()
    bus.publish(interface_name, emit)
    # Setup repeating timer
    GLib.timeout_add_seconds(interval=1, function=repeating_timer) 
    # Run loop with graceful ctrl C exiting.
    try:
        loop.run()
    except KeyboardInterrupt as e:
        loop.quit()
        print("\nExit by Control C")
