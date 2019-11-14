#!/usr/bin/env python3
import sys, curses, time
from curses import wrapper
from canard import can
from canard.hw import socketcan

MAX_TABLE_SIZE = 64

def display_node(screen, line, node):
    for i in range(0, 100): screen.addstr(line, i, " ")
    screen.addstr(line, 0, "Node(" + str(node.id) + ")\t| Dlc(" + str(node.dlc) + ") | Type(" + str(node.frame_type) + ") | Ext(" + str(node.is_extended_id) + ") | Data" + str(node.data))

def main(stdscr):
    table = {}

    # Open the screen
    stdscr = curses.initscr()
    curses.noecho()
    curses.cbreak()

    dev = socketcan.SocketCanDev('vcan0')
    dev.start()

    while(True):
        stdscr.addstr(0, 0, str(time.ctime()) + " | Table size: " + str(len(table)) + " nodes")
        res = dev.recv()
        if(len(table) < MAX_TABLE_SIZE): table[res.id] = res
        for i, id in enumerate(sorted(table)): display_node(stdscr, i + 2, table[id])
        stdscr.refresh()

    # Close the screen
    curses.nocbreak()
    stdscr.keypad(False)
    curses.echo()
    curses.endwin()

if __name__ == '__main__': wrapper(main)
