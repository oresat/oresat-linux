#!/usr/bin/env python3
"""Quick CLI to get the EEPROM values from the Raspi Pico via serial."""

from argparse import ArgumentParser

import serial  # type: ignore

paser = ArgumentParser()
paser.add_argument("port", help="usb port path")
args = paser.parse_args()

ser = serial.Serial(
    port=args.port,
    baudrate=115200,
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.EIGHTBITS,
    timeout=1,
)

raw = b""
while len(raw) == 0:
    try:
        raw = ser.readline()[:-2]  # remove trailing \r\n
        data = raw.decode()
        print(data)
    except Exception as e:
        print(e)
