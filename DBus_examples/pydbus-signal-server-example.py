#!/usr/bin/env python3


from pydbus.generic import signal
from pydbus import SystemBus
from gi.repository import GLib
import random


INTERFACE_NAME = "org.OreSat.Example"


class Test_Server(object):
    dbus = """
    <node>
        <interface name="org.OreSat.Example">
            <signal name="DataSignal">
                <arg type='i'/>
                <arg type='d'/>
            </signal>
            <signal name="HelloSignal">
                <arg type='s'/>
            </signal>
        </interface>
    </node>
    """


    DataSignal = signal()
    HelloSignal = signal()


test_server = Test_Server()


def send_Hello_signal():
    emit.HelloSignal("Hello world!")
    print("Hello world")
    return True # must return true if use timeout_add_secounds


def send_data_signal():
    random_int = random.randint(0,1000)
    random_double = random.uniform(0,100.0)

    emit.DataSignal(random_int, random_double)

    print(random_int, random_double)

    return True # must return true if use timeout_add_secounds


if __name__=="__main__":
    bus = SystemBus() # connect to bus
    loop = GLib.MainLoop()

    # Setup server to emit signals over the DBus
    bus.publish(INTERFACE_NAME, test_server)

    # loop signal emit
    GLib.timeout_add_seconds(interval=1, function=send_Hello_signal)
    GLib.timeout_add_seconds(interval=3, function=send_data_signal)

    # Run loop with graceful ctrl C exiting.
    try:
        loop.run()
    except KeyboardInterrupt as e:
        loop.quit()

