#!/usr/bin/env python3

import time
from canard.hw import socketcan
from frame_data import CursesFrame

def main():
    dev = socketcan.SocketCanDev("vcan0")
    dev.start()

    # while True:
    frame = FrameData(dev.recv(), dev.ndev)

    for i in frame.items():
        print(i)

    time.sleep(1)


if __name__ == "__main__": main()
