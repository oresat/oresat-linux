#!/usr/bin/env python3

from pydbus.generic import signal
from pydbus import SystemBus
from gi.repository import GLib

INTERFACE_NAME = "org.OreSat.Updater"

# ----------------------------------------------------------------------------
# Server

""" status enum
-# error
0 nothing
1 updating
2 stopped
"""

class dbus_interface(object):
    dbus = """
    <node>
        <interface name="org.example.project.oresat">
            <method name='Update'>
                <arg type='ss' name='file_path' direction='in'/>
                <arg type='i' name='output' direction='out'/>
            </method>
            <method name='StopUpdate' />
            <signal name="UpdaterStatus">
                <arg type='i'/>
            </signal>
        </interface>
    </node>
    """

    # signal(s)
    UpdaterStatus = signal()

    def sendUpdaterStatus(state):
        emit.UpdaterStatus(state)
        print(state)
        return True # for timeout_add_secounds

    # method(s)
    def Update(self, board, file_path):
        # To start updaing process with file_path
        print("Recieved: {} {}".format(board, file_path))
        return 1

    def StopUpdate(self):
        # stop update safely
        print("Stopping update")
        return 1

if __name__=="__main__":
    bus = SystemBus()
    loop = GLib.MainLoop()

    emit = dbus_interface()
    bus.publish(INTERFACE_NAME, emit)
    
    # signals
    GLib.timeout_add_seconds(interval=1, function=sendUpdaterStatus) 

    try:
        loop.run()
    except KeyboardInterrupt as e:
        loop.quit()
        print("\nExit by Control C")

