#!/usr/bin/env python3
"""Quick CLI to get EEPROM field values from user and send them to the Raspi Pico via serial."""

import json
from argparse import ArgumentParser
from datetime import datetime

import serial  # type: ignore

BOARD_NAMES = [
    "c3",
    "star-tracker",
    "gps",
    "dxwifi",
    "cfc",
]

paser = ArgumentParser()
paser.add_argument("port", help="usb port path")
paser.add_argument("name", choices=BOARD_NAMES, help="the card name")
paser.add_argument("id", type=int, help="the card id")
paser.add_argument("major", type=int, help="the card revision major")
paser.add_argument("minor", type=int, help="the card revision minor")
paser.add_argument(
    "week", type=int, help="the week number the card was made or 0 for current week"
)
paser.add_argument(
    "year", type=int, help="the year number the card was made or 0 for current year"
)
args = paser.parse_args()

now = datetime.now()
week = now.isocalendar()[1] if args.week == 0 else args.week
year = now.year if args.year == 0 else args.year
year %= 100

data = {
    "name": args.name,
    "id": args.id,
    "major": args.major,
    "minor": args.minor,
    "year": year,
    "week": week,
}

raw = json.dumps(data)

ser = serial.Serial(
    port=args.port,
    baudrate=115200,
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.EIGHTBITS,
    timeout=1,
)

try:
    ser.write(raw.encode())
    message = ser.readline()
    print(message.decode())
except Exception as e:
    print(e)
