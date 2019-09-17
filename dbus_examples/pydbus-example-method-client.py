#!/usr/bin/env python3

from pydbus import SessionBus

interface_name = "org.example.project.oresat"

bus = SessionBus()
the_object = bus.get(interface_name)

print("")

print("Command Method Called")
reply = the_object.Command("THIS IS A TEST")
print("Returned: %s\n" % reply)

print("Quit Method Called\n")
the_object.Quit()
