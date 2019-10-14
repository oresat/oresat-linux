#!/usr/bin/env python3

from pydbus.generic import signal
from pydbus import SystemBus
from gi.repository import GLib

INTERFACE_NAME = "org.example.project.oresat"

# ----------------------------------------------------------------------------
# Server


class Server_XML(object):
    dbus = """
    <node>
        <interface name="org.example.project.oresat">
            <method name='Update'>
                <arg type='s' name='file_path' direction='in'/>
                <arg type='d' name='output' direction='out'/>
            </method>
            <method name='Quit'/>
        </interface>
    </node>
    """

    def Update(self, file_path):
        # To start updaing process with file_path
        print("Recieved: {}".format(file_path))
        return 1

    def Quit(self):
        # Removes this object from the DBUS connection and exits
        loop.quit()

class dbus_interface():
    bus = SystemBus() # connect to bus
    loop = GLib.MainLoop() # only used by server
    emit = Server_XML() # Setup server to emit signals over the DBus

    def init(self):
        bus.publish(INTERFACE_NAME, self.emit)

    def run(self):
        self.loop.run()

    def end(self):
        self.loop.quit()

if __name__=="__main__":
    di = dbus_interface
    di.init()
    di.run()
