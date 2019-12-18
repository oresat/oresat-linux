#!/usr/bin/env python3

from pydbus.generic import signal
from pydbus import SystemBus
from gi.repository import GLib

import linux-updater


INTERFACE_NAME = "org.OreSat.Updater"
linux-updater = LinuxUpdater()


class dbus_interface(object):
    dbus = """
    <node>
        <interface name="org.OreSat.Updater">
            <method name='AddUpdateFile'>
                <arg type='s' name='file_path' direction='in'/>
                <arg type='i' name='output' direction='out'/>
            </method>
            <method name='StartUpdate'>
                <arg type='i' name='output' direction='out'/>
            </method>
            <method name='EmergencyStopUpdate' />
            <signal name="UpdaterStatus">
                <arg type='i'/>
            </signal>
        </interface>
    </node>
    """

    # signal(s)
    UpdaterStatus = signal()

    def sendUpdaterStatus(self):
        self.UpdaterStatus(linux-updater.state)
        return True # for timeout_add_secounds


    # method(s)
    def AddUpdateFile(self, file_path):
        """ To start updaing process with file_path """
        linux-updater.add_update_file(file_path)
        return 1


    def StartUpdate(self):
        """ To start updaing process with file_path """
        linux-updater.update_async()
        return 1


    def StopUpdate(self):
        # stop update safely
        return 1


start_linux_updater():
    bus = SystemBus()
    loop = GLib.MainLoop()

    emit = dbus_interface()
    bus.publish(INTERFACE_NAME, emit)
    
    # signals
    GLib.timeout_add_seconds(interval=1, function=emit.sendUpdaterStatus) 

    try:
        loop.run()
    except KeyboardInterrupt as e:
        loop.quit()
        print("\nExit by Control C")

