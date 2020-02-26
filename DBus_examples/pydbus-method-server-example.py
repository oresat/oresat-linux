#!/usr/bin/env python3


from pydbus import SystemBus
from gi.repository import GLib


INTERFACE_NAME = "org.OreSat.Example"


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


if __name__=="__main__":
    bus = SystemBus() # connect to bus
    loop = GLib.MainLoop()

    test_server = Test_Server()
    bus.publish(INTERFACE_NAME, test_server)

    try:
        loop.run()
    except KeyboardInterrupt as e:
        loop.quit()
