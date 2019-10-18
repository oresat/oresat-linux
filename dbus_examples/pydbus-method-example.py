#!/usr/bin/env python3

from pydbus.generic import signal
from pydbus import SystemBus
from gi.repository import GLib
import sys
import random

INTERFACE_NAME = "org.example.project.oresat"
bus = SystemBus() # connect to bus
loop = GLib.MainLoop() # only used by server


# ----------------------------------------------------------------------------
# Client

def client():
    the_object = bus.get(INTERFACE_NAME)

    print("Calling Hello method")
    reply = the_object.Hello("Hello Server!")
    print("Returned: %s\n" % reply)

    print("Calling Multiply method")
    reply = the_object.Multiply(14, 23)
    print("Returned: {}\n".format(reply))

    print("Quit Method Called")
    the_object.Quit()


# ----------------------------------------------------------------------------
# Server


class Server_XML(object):
    dbus = """
    <node>
        <interface name="org.example.project.oresat">
            <method name='Hello'>
                <arg type='s' name='input' direction='in'/>
                <arg type='s' name='output' direction='out'/>
            </method>
            <method name='Multiply'>
                <arg type='i' name='input1' direction='in'/>
                <arg type='i' name='input2' direction='in'/>
                <arg type='i' name='output' direction='out'/>
            </method>
            <method name='Quit'/>
        </interface>
    </node>
    """

    def Hello(self, input):
        """to pass strings between server/client"""
        print("Hello method called")
        print("Recieved: {}".format(input))
        return "Hello Client!"

    def Multiply(self, input1, input2):
        """multipy two inputs together and return it"""
        print("Hello method called")
        print("Recieved: {} {}".format(input1, input2))
        return input1 * input2

    def Quit(self):
        """removes this object from the DBUS connection and exits"""
        loop.quit()

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

