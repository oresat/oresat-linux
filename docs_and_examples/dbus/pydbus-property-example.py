#!/usr/bin/env python3

from pydbus.generic import signal
from pydbus import SystemBus
from gi.repository import GLib
import sys
import random
import time

INTERFACE_NAME = "org.example.project.oresat"
bus = SystemBus() # connect to bus
loop = GLib.MainLoop() # only used by server


# ----------------------------------------------------------------------------
# Client

def client():
    the_object = bus.get(INTERFACE_NAME)

    while True:
        print(the_object.Test1)
        print(the_object.Test2)
        the_object.Test2+=1
        time.sleep(1)



# ----------------------------------------------------------------------------
# Server


class Server_XML(object):
    dbus = """
    <node>
        <interface name="org.example.project.oresat">
            <property name="Test1" type="d" access="read"/>
            <property name="Test2" type="u" access="readwrite"> 
                <annotation name="org.freedesktop.DBus.Property.EmitsChangedSignal" value="true"/>
            </property>
        </interface>
    </node>
    """
    def __init__(self):
        self._Test1 = 12.3
        self._Test2 = 1
    
    @property
    def Test1(self):
        return self._Test1

    @property
    def Test2(self):
        return self._Test2

    @Test2.setter
    def Test2(self, value):
        self._Test2 = value
        self.PropertiesChanged(INTERFACE_NAME, {"Test2": self._Test2}, [])

    PropertiesChanged = signal()

def server():
    # Setup server to emit signals over the DBus
    emit = Server_XML()
    bus.publish(INTERFACE_NAME, emit)

    # Run loop with graceful ctrl C exiting.
    try:
        loop.run()
    except KeyboardInterrupt as e:
        loop.quit()
        print("\nExit by Control C")


# ----------------------------------------------------------------------------


if __name__=="__main__":
    if(sys.argv[1] == "server"):
        server()
    elif(sys.argv[1] == "client"):
        client()
    else:
        print("Input error\n python3 pydbus-method-example.py <Mode>\n Where <Mode> is server or client\n");

