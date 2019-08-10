#!/usr/bin/env python3

# Python script to call the methods of the DBUS Test Server

from pydbus import SessionBus

#get the session bus
bus = SessionBus()
#get the object
the_object = bus.get("net.lew21.pydbus.ClientServerExample")

reply1, reply2, reply3 = the_object.GetData()
print(reply1, reply2, reply3)

reply = the_object.Command("Hello")
print(reply)

the_object.Quit()
