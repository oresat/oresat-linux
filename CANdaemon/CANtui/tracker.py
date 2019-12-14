#!/usr/bin/env python3
import sys, curses, time, os
from curses import wrapper
from canard import can
from canard.hw import socketcan
from ftfutils import Color, CString

MAX_TABLE_SIZE = 64 # Number of nodes to keep in the table
MAX_KEEP_ALIVE = 60 # Time in seconds to keep a nod alive

class FrameData(can.Frame):
    def __init__(self, src=None):
        self.modif = time.time()
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
        ret = str("Node(" + str(self.id) + ") | Dlc(" + str(self.dlc) + ") | Type(" + str(self.frame_type) + ") | Ext(" + str(self.is_extended_id) + ") | Data[ ")
        for d in self.data:
            ds = str(hex(d))
            ret += str(ds[2:] + " ")
        ret += "] | LastModified("
        return ret

    def curses_print(self, screen, line):
        screen.addstr(0, line, "Node(" + str(self.id) + ")", curses.color_pair(1))
        screen.addstr(" | ")
        screen.addstr(0, line, "Dlc(" + str(self.dlc) + ")", curses.color_pair(1))

    def update(self, frame):
        self.alive = True
        self.modif = time.time()
        self.is_extended_id = frame.is_extended_id
        self.dlc = frame.dlc
        self.data = frame.data
        self.frame_type = frame.frame_type

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

    if(node.is_alive()):
        screen.addstr(str(int(time.time() - int(node.modif))) + "s", curses.color_pair(1))
        screen.addstr(")")
    else:
        screen.addstr(str(time.ctime(node.modif)), curses.color_pair(2))
        screen.addstr(") | ")
        screen.addstr("STALE NODE", curses.color_pair(3))

def main(stdscr):
    table = {}

    # Init curses colors
    curses.init_pair(1, curses.COLOR_GREEN, curses.COLOR_BLACK)
    curses.init_pair(2, curses.COLOR_YELLOW, curses.COLOR_BLACK)
    curses.init_pair(3, curses.COLOR_RED, curses.COLOR_BLACK)

    # Open the screen
    stdscr = curses.initscr()
    curses.noecho()
    curses.cbreak()

    dev = socketcan.SocketCanDev('vcan0')
    dev.start()

    while(True):
        stdscr.addstr(0, 0, str(time.ctime()), curses.color_pair(1))
        stdscr.addstr(" | ")
        stdscr.addstr("Table size: " + str(len(table)) + " nodes", curses.color_pair(2))
        res = dev.recv()

        if(res.id in table): table[res.id].update(res)
        elif(len(table) < MAX_TABLE_SIZE): table[res.id] = FrameData(res)

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
