#!/usr/bin/env python3

# Python DBUS Test Server
# runs until the Quit() method is called via DBUS

from gi.repository import GLib
from pydbus import SessionBus

loop = GLib.MainLoop()

class MyDBUSService(object):
    """
        <node>
            <interface name='net.lew21.pydbus.ClientServerExample'>
                <method name='GetData'>
                    <arg type='s' name='test_string' direction='out'/>
                    <arg type='d' name='test_float' direction='out'/>
                    <arg type='i' name='test_int' direction='out'/>
                </method>
                <method name='Command'>
                    <arg type='s' name='command' direction='in'/>
                    <arg type='s' name='response' direction='out'/>
		</method>
		<method name='Quit'/>
            </interface>
        </node>
    """

    def GetData(self):
        """returns its data"""
        return ("Hello, World!", 1.2, 10)

    def Command(self, s):
        """to pass command from C3 card"""
        print(s)
        return "done"

    def Quit(self):
        """removes this object from the DBUS connection and exits"""
        loop.quit()

bus = SessionBus()
bus.publish("net.lew21.pydbus.ClientServerExample", MyDBUSService())
loop.run()
