#!/usr/bin/env python3
import curses, time
from canard.hw import socketcan
from frame_data import FrameData, HeartbeatFrame
from frame_table import FrameTable

DEBUG = False

def display_hearbeat():
    pass

def main(window):
    table_size = 8

    # Init the colors
    # Color pair 0: Default White/Black
    curses.init_pair(1, curses.COLOR_GREEN, curses.COLOR_BLACK)
    curses.init_pair(2, curses.COLOR_YELLOW, curses.COLOR_BLACK)
    curses.init_pair(3, curses.COLOR_RED, curses.COLOR_BLACK)
    curses.init_pair(4, curses.COLOR_MAGENTA, curses.COLOR_BLACK)
    curses.init_pair(5, curses.COLOR_BLACK, curses.COLOR_GREEN)
    curses.init_pair(6, curses.COLOR_BLACK, curses.COLOR_CYAN)

    # Table Setup
    tables = [ FrameTable("Heartbeat", table_size),
               FrameTable("RDO's", table_size),
               FrameTable("Miscellaneous", table_size) ]

    # Open the standard output
    stdscr = curses.initscr()
    curses.noecho()
    curses.cbreak()
    curses.curs_set(0)

    # Open the sockets
    dev_names = [ "vcan0" ]
    # dev_names = [ "vcan0",
    #               "vcan1" ]
    devs = []
    for name in dev_names:
        dev = socketcan.SocketCanDev(name)
        dev.start()
        devs.append(dev)

    # Start the main loop
    while True:
        # Get screen height/width
        y, x = stdscr.getmaxyx()

        # Determine if the data can fit on screen
        if(((table_size + 2) * len(tables)) + 2 > y): raise OverflowError("Data is too large for this screen! (try reducing the table size)")

        # Display the banner (time | active devices)
        stdscr.addstr(0, 0, "Time: ")
        stdscr.addstr(time.ctime(), curses.color_pair(3))
        stdscr.addstr("\tDevices: [")
        for dev in devs:
            stdscr.addstr(" " + str(dev.ndev), curses.color_pair(2))
        stdscr.addstr(" ]")

        if(DEBUG): stdscr.addstr(" (w: " + str(x) + ", h: " + str(y) + ")")

        # Recieve messages from the CAN bus devices in a round-robin fashion
        for dev in devs:
            # Set a timeout for recieving data, to enforce RR check
            # dev.settimeout(1)
            raw_frame = dev.recv()
            new_frame = None
            id = raw_frame.id

            # Figure out the right table to put the new data in
            if(id >= 700 and id < 800):
                new_frame = FrameData(raw_frame, dev.ndev)
                tnum = 0
            elif(id >= 500 and id < 700):
                new_frame = FrameData(raw_frame, dev.ndev)
                tnum = 1
            else:
                new_frame = FrameData(raw_frame, dev.ndev)
                tnum = 2

            new_frame.id %= 100
            tables[tnum].add(new_frame)

            for i, table in enumerate(tables):
                # Calculate the header padding
                table_header = str(table.name) + ": " \
                    + "(" + str(len(table.table)) + " nodes)"
                table_padding = " " * int(x - len(table_header))

                # Display the header
                stdscr.addstr((i * table_size) + i * 2 + 2, 0,
                        table_header + table_padding, curses.color_pair(5))

                # Calculate the label padding
                label = "Id:\tDevice\tStatus\tData:"
                label_padding = " " * int(x - len(label) - 6)
                stdscr.addstr(label + label_padding, curses.color_pair(6))

                # Display all of the nodes in the table
                #   in sorted order by node id
                for k in sorted(table.table.keys()): # Get list of sorted keys
                    frame = table.table[k]

                    # Display the node with color
                    stdscr.addstr(str(frame.id) + "\t" + str(frame.ndev) + "\t")
                    if(frame.is_dead()): stdscr.addstr("DEAD", curses.color_pair(3))
                    elif(frame.is_stale()): stdscr.addstr("STALE", curses.color_pair(2))
                    else: stdscr.addstr("ALIVE", curses.color_pair(1))

                    stdscr.addstr("\t[")
                    for val in frame.data:
                        stdscr.addstr(" " + str(hex(val)), curses.color_pair(4))
                    stdscr.addstr(" ]\n")
            stdscr.refresh()

    # Close the standard input
    curses.nocbreak()
    stdscr.keypad(False)
    curses.echo()
    curses.curs_set(1)
    curses.endwin()

if __name__ == "__main__":
    try: curses.wrapper(main)
    except OverflowError as e: print("fatal error: " + str(e))
