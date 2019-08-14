#!/usr/bin/env python3

from pydbus import SessionBus

interface_name = "org.example.project_1.server_1"

bus = SessionBus()
the_object = bus.get(interface_name)

reply = the_object.Command("THIS IS A TEST")
print(reply)

