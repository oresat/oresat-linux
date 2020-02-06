#!/usr/bin/env python3
import curses
from canard.hw import socketcan
from frame_data import FrameData
from frame_table import FrameTable

def main(argv):
    table_size = 16

    # Table Setup
    tables = [
        FrameTable("Heartbeat", table_size),
        FrameTable("RDO's", table_size),
        FrameTable("Miscellaneous", table_size)
    ]

    # Open the standard output
    stdscr = curses.initscr()
    curses.noecho()
    curses.cbreak()

    # Open the sockets
    dev_names = [
        "vcan0",
        "vcan1"
    ]
    devs = []
    for name in dev_names:
        dev = socketcan.SocketCanDev(name)
        dev.start()
        devs.append(dev)

    # Display the active devices
    stdscr.addstr("Devices: [")
    for dev in devs: stdscr.addstr(" " + str(dev.ndev))
    stdscr.addstr(" ]")

    # Start the main loop
    while True:
        # Recieve messages from the CAN bus devices in a round-robin fashion
        for dev in devs:
            # Set a timeout for recieving data, to enforce RR check
            # dev.settimeout(1)
            new_frame = FrameData(dev.recv())
            id = new_frame.id

            if(id >= 700 and id < 800): tables[0].add(new_frame)
            elif(id >= 500 and id < 700): tables[1].add(new_frame)
            else: tables[2].add(new_frame)

            for i, table in enumerate(tables):
                # Display the header
                stdscr.addstr((i * table_size) + i * 2 + 1, 0, str(table.name) + ": (" + str(len(table.table)) + ")")
                stdscr.addstr("\n\tId:\tDevice\tStatus\tData:")

                # Display all of the nodes in the table
                #   in sorted order by node id
                for k in sorted(table.table.keys()): # Get a list of sorded keys
                    frame = table.table[k] # Get the value of the corresponding key

                    # Determine the state of the node
                    if(frame.is_stale()): status = "STALE"
                    elif(frame.is_dead()): status = "DEAD"
                    else: status = "ALIVE"

                    # Display the node
                    stdscr.addstr("\n\t" + str(frame.id)
                                         + "\t" + str(dev.ndev)
                                         + "\t" + status
                                         + "\t[")
                    for val in frame.data:
                        stdscr.addstr(" " + str(hex(val)))
                    stdscr.addstr(" ]")
                stdscr.refresh()

    # Close the standard input
    curses.nocbreak()
    stdscr.keypad(False)
    curses.echo()
    curses.endwin()

if __name__ == "__main__": curses.wrapper(main)
