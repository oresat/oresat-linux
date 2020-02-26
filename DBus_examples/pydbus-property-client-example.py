#!/usr/bin/env python3


from pydbus import SystemBus
import time


INTERFACE_NAME = "org.OreSat.Example"


if __name__=="__main__":
    bus = SystemBus() # connect to bus
    the_object = bus.get(INTERFACE_NAME)

    while True:
        print(the_object.Test1)
        print(the_object.Test2)
        the_object.Test2 += 1
        time.sleep(1)


