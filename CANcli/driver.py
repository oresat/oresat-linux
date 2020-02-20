#!/usr/bin/env python3
import curses, time, sys
from canard.hw import socketcan
from frame_data import FrameData
from frame_table import FrameTable, FrameType

DEBUG = False
SCROLL_LINE = 0

def usage():
    print("usage: ./driver <devices>\n")
    sys.exit(-1)

def display_banner(screen, table, banner):
    height, width = screen.getmaxyx() # Get screen height/width

    # Calculate the header padding
    table_header = str(table.name) + ": " + "(" + str(len(table))
    if(table.max_table_size is not None): table_header += "/" + str(table.max_table_size)
    table_header += " nodes)"
    table_padding = " " * int(width - len(table_header))

    # Calculate the banner padding
    banner_padding = " " * int(width - len(banner) - 7)

    # Display the banner
    screen.addstr(table_header + table_padding, curses.color_pair(5))
    screen.addstr(banner + banner_padding, curses.color_pair(6))
    return 2

def display_heartbeat(screen, table, budget):
    lcount = display_banner(screen, table, "Id\tDevice\tStatus  :")

    for k in sorted(table.table.keys()): # Get list of sorted keys
        lcount += 1
        if(lcount < budget):
            frame = table.table[k]

            # Display the node with color
            screen.addstr(str(hex(frame.id)) + "\t" + str(frame.ndev) + "\t")
            if(frame.is_dead()): screen.addstr("DEAD", curses.color_pair(3))
            elif(frame.is_stale()): screen.addstr("STALE", curses.color_pair(2))
            else: screen.addstr(str(hex(frame.data[0])), curses.color_pair(1))
            screen.addstr("\n")
        else: break
    return lcount

def display_rdo(screen, table, budget):
    lcount = display_banner(screen, table, "Id\tDevice\tStatus\tData")

    for k in sorted(table.table.keys()): # Get list of sorted keys
        lcount += 1
        if(lcount < budget):
            frame = table.table[k]

            # Display the node with color
            screen.addstr(str(hex(frame.id)) + "\t" + str(frame.ndev) + "\t")
            if(frame.is_dead()): screen.addstr("DEAD", curses.color_pair(3))
            elif(frame.is_stale()): screen.addstr("STALE", curses.color_pair(2))
            else: screen.addstr("ALIVE", curses.color_pair(1))

            screen.addstr("\t[")
            for val in frame.data:
                screen.addstr(" " + str(hex(val)), curses.color_pair(4))
            screen.addstr(" ]\n")
        else: break
    return lcount

def display_misc(screen, table, budget):
    lcount = display_banner(screen, table, "Id\tDevice\tStatus\tData")

    for k in sorted(table.table.keys()): # Get list of sorted keys
        lcount += 1
        if(lcount < budget):
            frame = table.table[k]

            # Display the node with color
            screen.addstr(str(hex(frame.id)) + "\t" + str(frame.ndev) + "\t")
            if(frame.is_dead()): screen.addstr("DEAD", curses.color_pair(3))
            elif(frame.is_stale()): screen.addstr("STALE", curses.color_pair(2))
            else: screen.addstr("ALIVE", curses.color_pair(1))

            screen.addstr("\t[")
            for val in frame.data:
                screen.addstr(" " + str(hex(val)), curses.color_pair(4))
            screen.addstr(" ]\n")
        else: break
    return lcount

def main(window=None):
    if(len(sys.argv) < 2): usage()
    dev_names = sys.argv[1:]

    # Init the colors
    # Color pair 0: Default White/Black
    curses.init_pair(1, curses.COLOR_GREEN, curses.COLOR_BLACK)
    curses.init_pair(2, curses.COLOR_YELLOW, curses.COLOR_BLACK)
    curses.init_pair(3, curses.COLOR_RED, curses.COLOR_BLACK)
    curses.init_pair(4, curses.COLOR_MAGENTA, curses.COLOR_BLACK)
    curses.init_pair(5, curses.COLOR_BLACK, curses.COLOR_GREEN)
    curses.init_pair(6, curses.COLOR_BLACK, curses.COLOR_CYAN)

    # Table Setup
    tables = [ FrameTable("Heartbeat", FrameType.HEARBEAT),
               FrameTable("SDO's", FrameType.RDO),
               FrameTable("Miscellaneous", FrameType.MISC, 32) ]

    # Open the standard output
    stdscr = curses.initscr()
    curses.noecho()
    curses.cbreak()
    curses.curs_set(0)

    # Open the socketsC
    devs = []
    for name in dev_names:
        dev = socketcan.SocketCanDev(name)
        dev.start()
        devs.append([True, dev])

    # Start the main loop
    while True:
        used_height = 1
        height, width = stdscr.getmaxyx() # Get screen height/width

        # Display the banner (time | active devices)
        stdscr.addstr(0, 0, "Time: ")
        stdscr.addstr(time.ctime(), curses.color_pair(3))
        stdscr.addstr("\tDevices: [")
        for dev in devs:
            if(dev[0]): cp = 1
            else: cp = 3
            stdscr.addstr(" " + str(dev[1].ndev), curses.color_pair(cp))
        stdscr.addstr(" ]\n")

        if(DEBUG): stdscr.addstr(" (w: " + str(width) + ", h: " + str(height) + ")")

        # Recieve messages from the CAN bus devices in a round-robin fashion
        for dev in devs:
            # Set a timeout for recieving data, to enforce RR check
            # dev.settimeout(1)
            try:
                raw_frame = dev[1].recv()
                dev[0] = True
            except OSError as e:
                dev[0] = False
                continue

            new_frame = None
            id = raw_frame.id

            #
            # Hearbeat ID's: 0x701 - 0x7FF
            # SDO ID's:
            #   tx: 0x581 - 0x5FF
            #   rx: 0x601 - 0x67F
            #
            # Figure out the right table to put the new data in
            if(id >= 0x701 and id <= 0x7FF): # Heartbeats
                new_frame = FrameData(raw_frame, dev[1].ndev)
                new_frame.id -= 0x700
                tables[0].add(new_frame)
            elif(id >= 0x581 and id <= 0x5FF): # SDO tx
                new_frame = FrameData(raw_frame, dev[1].ndev)
                new_frame.id -= 0x580
                tables[1].add(new_frame)
            elif(id >= 0x601 and id <= 0x67F): # SDO rx
                new_frame = FrameData(raw_frame, dev[1].ndev)
                new_frame.id -= 0x600
                tables[2].add(new_frame)
            else: # Others
                new_frame = FrameData(raw_frame, dev[1].ndev)
                tables[2].add(new_frame)

            for table in tables:
                if(len(table) > 0):
                    if(table.type == FrameType.HEARBEAT): used_height += display_heartbeat(stdscr, table, height - used_height)
                    elif(table.type == FrameType.RDO): used_height += display_rdo(stdscr, table, height - used_height)
                    else: used_height += display_misc(stdscr, table, height - used_height)
                stdscr.refresh()

    # Close the standard input
    curses.nocbreak()
    stdscr.keypad(False)
    curses.echo()
    curses.curs_set(1)
    curses.endwin()

if __name__ == "__main__":
    try: curses.wrapper(main)
    # try: main()
    except OverflowError as e: print("fatal error: " + str(e))
