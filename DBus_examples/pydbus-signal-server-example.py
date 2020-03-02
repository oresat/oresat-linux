#!/usr/bin/env python3


from pydbus.generic import signal
from pydbus import SystemBus
from gi.repository import GLib
import threading, time, random


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


    def __init__(self):
        self._Test1 = random.randint(0,1000)
        self._Test2 = random.uniform(0,100.0)
        self.__running = True
        self.__working_thread = threading.Thread(target=self.__working_loop)
        self.__working_thread.start() # start working thread


    def __working_loop(self):
        data_time_out = 0

        while (self.__running == True):
            if data_time_out >= 3:
                self._Test1 = random.randint(0,1000)
                self._Test2 = random.uniform(0,100.0)
                self.DataSignal(self._Test1, self._Test2)
                print(self._Test1, self._Test2)
                data_time_out = 0

            self.HelloSignal("Hello World!")
            print("Hello World!")

            data_time_out += 1
            time.sleep(1)


    def quit(self):
        self.__running = False


if __name__=="__main__":
    bus = SystemBus() # connect to bus
    loop = GLib.MainLoop()
    test_server = Test_Server()

    # Setup server to emit signals over the DBus
    bus.publish(INTERFACE_NAME, test_server)

    # Run loop with graceful ctrl C exiting.
    try:
        loop.run()
    except KeyboardInterrupt as e:
        test_server.quit()
        loop.quit()

