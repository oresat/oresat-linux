#!/usr/bin/env python3

from pydbus.generic import signal
from pydbus import SystemBus
from gi.repository import GLib
import sys
import random


INTERFACE_NAME = "org.OreSat.Example"


# ----------------------------------------------------------------------------
# Client


def cb_server_signal_emission(*args):
    """
    Callback on emitting signal from server
    """
    print("Data: ", args[4])


def client_main():
    bus = SystemBus() # connect to bus
    # Subscribe to bus to monitor for all server signals emissions
    bus.subscribe(iface = INTERFACE_NAME,
                  signal_fired = cb_server_signal_emission)
    # Run loop with graceful ctrl C exiting.
    try:
        loop.run()
    except KeyboardInterrupt as e:
        loop.quit()


# ----------------------------------------------------------------------------
# Server


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


def server_main():
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
        print("\nExit by Control C")


# ----------------------------------------------------------------------------

def usage():
    print("Input error\n python3 pydbus-signal-example.py <Mode>\n Where <Mode> is server or client\n");
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

