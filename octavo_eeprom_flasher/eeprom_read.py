#!/usr/bin/env python3
"""Quick CLI to get the EEPROM values from the Raspi Pico via serial."""

import json
from argparse import ArgumentParser

import serial  # type: ignore

paser = ArgumentParser()
paser.add_argument("port", help="usb port path")
args = paser.parse_args()

raw = json.dumps({"direction": "read"}).encode()

ser = serial.Serial(
    port=args.port,
    baudrate=115200,
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.EIGHTBITS,
    timeout=1,
)

try:
    ser.write(raw)
    message = ser.readline()
    if len(message) == 0:
        print("no message")
    else:
        print(message.decode())
except Exception as e:
    print(e)
