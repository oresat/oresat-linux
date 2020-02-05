#!/usr/bin/env python3
from canard.hw import socketcan
from frame_data import FrameData
from frame_table import FrameTable
from ftfutils import log, Mode

def main():
    # Table Setup
    hb_table = FrameTable("Heartbeat", 4)
    sdo_table = FrameTable("RDO's", 4)
    misc_table = FrameTable("Miscellaneous", 4)

    # Open the socket to vCAN
    dev = socketcan.SocketCanDev('vcan0')
    dev.start()

    while True:
        new_frame = FrameData(dev.recv())
        id = new_frame.id

        if(id >= 700 and id < 800): hb_table.add(new_frame)
        elif(id >= 500 and id < 700): sdo_table.add(new_frame)
        else: misc_table.add(new_frame)

        log(Mode.DEBUG, str(hb_table) + "\n" + str(sdo_table) + "\n" + str(misc_table))

if __name__ == "__main__": main()
