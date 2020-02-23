#!/usr/bin/env python3

from pydbus.generic import signal
from pydbus import SystemBus
from gi.repository import GLib
import sys
import random
import time

INTERFACE_NAME = "org.OreSat.Example"


# ----------------------------------------------------------------------------
# Client

def client_main():
    bus = SystemBus() # connect to bus
    the_object = bus.get(INTERFACE_NAME)

    while True:
        print(the_object.Test1)
        print(the_object.Test2)
        the_object.Test2+=1
        time.sleep(1)



# ----------------------------------------------------------------------------
# Server


class Test_Server(object):
    dbus = """
    <node>
        <interface name="org.OreSat.Example">
            <property name="Test1" type="d" access="read">
                <annotation name="org.freedesktop.DBus.Property.EmitsChangedSignal" value="true"/>
            </property>
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

def server_main():
    bus = SystemBus() # connect to bus
    loop = GLib.MainLoop() # only used by server

    # Setup server to emit signals over the DBus
    test_server = Test_Server()
    bus.publish(INTERFACE_NAME, test_server)

    # Run loop with graceful ctrl C exiting.
    try:
        loop.run()
    except KeyboardInterrupt as e:
        loop.quit()
        print("\nExit by Control C")


# ----------------------------------------------------------------------------

def usage():
    print("Input error\n python3 pydbus-property-example.py <Mode>\n Where <Mode> is server or client\n")
    sys.exit(1)

if __name__=="__main__":
    if len(sys.arg != 2:
        usage()

    if(sys.argv[1] == "server"):
        server_main()
    elif(sys.argv[1] == "client"):
        client_main()
    else:
        usage()

