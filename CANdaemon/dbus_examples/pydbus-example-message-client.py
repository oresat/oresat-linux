#!/usr/bin/env python3

from pydbus import SessionBus

interface_name = "org.example.oresat"

bus = SessionBus()
the_object = bus.get(interface_name)

reply = the_object.Command("THIS IS A TEST")
print(reply)

