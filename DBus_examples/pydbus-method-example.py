#!/usr/bin/env python3

from pydbus import SystemBus
from gi.repository import GLib
import sys
import random

INTERFACE_NAME = "org.OreSat.Example"


# ----------------------------------------------------------------------------
# Client

def client_main():
    bus = SystemBus() # connect to bus
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


class Test_Server(object):
    dbus = """
    <node>
        <interface name="org.OreSat.Example">
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

def server_main():
    bus = SystemBus() # connect to bus
    loop = GLib.MainLoop() # only used by server

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
    print("Input error\n python3 pydbus-method-example.py <Mode>\n Where <Mode> is server or client\n");
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

