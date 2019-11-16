#!/usr/bin/env python3
import sys, curses, time, os
from curses import wrapper
from canard import can
from canard.hw import socketcan

MAX_TABLE_SIZE = 8 # Number of nodes to keep in the table
MAX_KEEP_ALIVE = 12 # Time in seconds to keep a nod alive

class FrameData(can.Frame):
    def __init__(self, modif, src=None):
        self.modif = modif
        self.alive = True

        if(src is None):
            self.is_extended_id = False
            self.dlc = -1
            self.id = -1
            self.data = []
            self.frame_type = -1
        else:
            self.is_extended_id = src.is_extended_id
            self.dlc = src.dlc
            self.id = src.id
            self.data = src.data
            self.frame_type = src.frame_type

    def __str__(self):
        ret = str("Node(" + str(self.id) + ")\t| Dlc(" + str(self.dlc) + ") | Type(" + str(self.frame_type) + ") | Ext(" + str(self.is_extended_id) + ") | Data[ ")
        for d in self.data:
            ds = str(hex(d))
            ret += str(ds[2:] + " ")
        ret += "] | LastModified("
        if(self.is_alive()): ret += str(int(time.time() - int(self.modif))) + "s)"
        else: ret += time.ctime() + "s) | STALE NODE"
        return ret

    def is_alive(self):
        if(int(time.time() - int(self.modif)) >= MAX_KEEP_ALIVE): self.alive = False
        return self.alive

def clear_line(screen, line):
    rows, cols = os.popen('stty size', 'r').read().split()
    clear = ""
    for i in range(0, int(cols)): clear += " "
    screen.addstr(line, 0, clear)

def display_node(screen, line, node):
    clear_line(screen, line)
    screen.addstr(line, 0, str(node))

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
        res = FrameData(time.time(), src=dev.recv())
        if(not (res.id in table) and (len(table) < MAX_TABLE_SIZE)): table[res.id] = res
        for i, id in enumerate(sorted(table)):
            table[id].is_alive()
            display_node(stdscr, i + 2, table[id])
        stdscr.refresh()

    # Close the screen
    curses.nocbreak()
    stdscr.keypad(False)
    curses.echo()
    curses.endwin()

if __name__ == '__main__': wrapper(main)
