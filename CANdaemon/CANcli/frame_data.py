#!/usr/bin/env python3

import curses, time
from curses import wrapper
from canard import can
from canard.hw import socketcan

MAX_TABLE_SIZE = 64 # Number of nodes to keep in the table
MAX_KEEP_ALIVE = 10 # Time in seconds to keep a nod alive

# Curses mini-utils
def clear_line(screen, line):
    clear = ""
    for i in range(0, int(COLS) - 1): clear += " "
    screen.addstr(line, 0, clear)

def cdata(screen, label, data, color_pair, parens=True):
    screen.addstr(str(label), curses.color_pair(0))
    if(parens): screen.addstr("(", curses.color_pair(0))
    screen.addstr(str(data), curses.color_pair(color_pair))
    if(parens): screen.addstr(")", curses.color_pair(0))

# Expanded frame data class
class FrameData(can.Frame):
    def __init__(self, src=None):
        self.labels = ["Node", "Dlc", "Type", "Ext", "Data", "LastModified"]
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

def main():
    e = FrameData()
    print()

if __name__ == "__main__": main()
