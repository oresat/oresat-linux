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

    # Open the socket to vCAN
    dev = socketcan.SocketCanDev('vcan0')
    dev.start()

    # Open the standard output
    stdscr = curses.initscr()
    curses.noecho()
    curses.cbreak()

    while True:
        new_frame = FrameData(dev.recv())
        id = new_frame.id

        if(id >= 700 and id < 800): tables[0].add(new_frame)
        elif(id >= 500 and id < 700): tables[1].add(new_frame)
        else: tables[2].add(new_frame)

        for i, table in enumerate(tables):

            stdscr.addstr((i * table_size) + i * 2, 0, str(table.name) + ": (" + str(len(table.table)) + ")")
            stdscr.addstr("\n\tId:\tStatus\tData:")
            for k in sorted(table.table.keys()):
                frame = table.table[k]
                if(frame.is_stale()): status = "STALE"
                elif(frame.is_dead()): status = "DEAD"
                else: status = "ALIVE"
                stdscr.addstr("\n\t" + str(frame.id)
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
