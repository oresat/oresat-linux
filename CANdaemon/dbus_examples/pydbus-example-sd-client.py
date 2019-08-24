#!/usr/bin/env python3

from pydbus import SessionBus

interface_name = "net.poettering.Calculator"

bus = SessionBus()
the_object = bus.get(interface_name)

print("")

print("Command Method Called")
reply = the_object.Multiply(10, 10)
print("Returned: %s\n" % reply)

