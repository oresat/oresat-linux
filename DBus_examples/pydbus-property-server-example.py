#!/usr/bin/env python3


from pydbus.generic import signal
from pydbus import SystemBus
from gi.repository import GLib
import threading, time


INTERFACE_NAME = "org.OreSat.Example"


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


    PropertiesChanged = signal()


    def __init__(self):
        self._Test1 = 12.3
        self._Test2 = 1
        self.__running = True
        self.__working_thread = threading.Thread(target=self.__working_loop)
        self.__working_thread.start() # start working thread


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


    def __working_loop(self):
        while (self.__running == True):
            self._Test1 += 1.0
            self.PropertiesChanged(INTERFACE_NAME, {"Test1": self._Test1}, [])
            time.sleep(1)


    def quit(self):
        self.__running = False


if __name__=="__main__":
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
        test_server.quit()

