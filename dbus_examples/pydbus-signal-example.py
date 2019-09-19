#!/usr/bin/env python3

from pydbus.generic import signal
from pydbus import SystemBus
from gi.repository import GLib
import sys
import random


INTERFACE_NAME = "org.example.project.oresat"
bus = SystemBus() # connect to bus
loop = GLib.MainLoop()


# ----------------------------------------------------------------------------
# Client


def cb_server_signal_emission(*args):
    """
    Callback on emitting signal from server
    """
    print("Data: ", args[4])


def client():
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


class Server_XML(object):
    dbus = """
    <node>
        <interface name="org.example.project.oresat">
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

emit = Server_XML()


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


def server():
    # Setup server to emit signals over the DBus
    bus.publish(INTERFACE_NAME, emit)

    # loop signal emit
    GLib.timeout_add_seconds(interval=1, function=send_signals) 

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

