#!/usr/bin/env python3


from pydbus import SystemBus
from gi.repository import GLib


INTERFACE_NAME = "org.OreSat.Example"


if __name__=="__main__":
    bus = SystemBus() # connect to bus
    the_object = bus.get(INTERFACE_NAME)

    print("Calling Hello method")
    reply = the_object.Hello("Hello Server!")
    print("Returned: %s\n" % reply)

    print("Calling Multiply method")
    reply = the_object.Multiply(14, 23)
    print("Returned: {}\n".format(reply))

    print("Quit Method Called")
    the_object.Quit()

