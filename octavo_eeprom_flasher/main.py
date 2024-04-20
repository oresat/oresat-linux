"""
This is MicroPython!

This script periodically reads and prints the board ID from the EERPROM in the OSD3358
and can be used to write a new board ID to the EEPROM as well.
"""

import json
import select
import sys
from machine import I2C, Pin, Timer
from time import sleep

I2C_ADDR = 0x50
EEPROM_ADDR = b"\x00\x00"
HEADER = b"\xAA\x55\x33\xEE"

BOARD_NAMES = {
    "pocketbeagle": "A335PBGL",
    "beagleboneblack": "A335BNLT",
    "c3": "A335OSC3",
    "star-tracker": "A335OSST",
    "gps": "A335OGPS",
    "dxwifi": "A335ODWF",
    "cfc": "A335OCFC",
}
BOARD_NAMES_REV = {name: key for key, name in BOARD_NAMES.items()}

led = Pin(25, Pin.OUT)
led.off()
led_timer = Timer(mode=Timer.PERIODIC, freq=1, callback=lambda t: led.toggle())

i2c = I2C(0, scl=Pin(17), sda=Pin(16), freq=400_000)

poll = select.poll()
poll.register(sys.stdin, select.POLLIN)


def eeprom_read() -> dict:
    try:
        i2c.writeto(I2C_ADDR, EEPROM_ADDR, False)
        raw = i2c.readfrom(I2C_ADDR, 28)
    except Exception:
        return {"error": "failed to read data from EEPROM"}

    data = raw[4:].decode()
    values = {
        "data": data,
        "board": data[:8],
        "name": BOARD_NAMES_REV.get(data[:8], "unknown"),
        "version": data[8:12],
        "week": int(data[12:14]),
        "year": int(data[14:16]),
        "id": int(data[20:24]),
    }
    return values


def eeprom_print(_timer: Timer):
    try:
        message = json.dumps(eeprom_read())
    except Exception as e:
        message = json.dumps({"error": str(e)})
    print(message)


read_timer = Timer(mode=Timer.PERIODIC, freq=1, callback=eeprom_print)


def eeprom_write(data: dict) -> dict:
    """Write the data to EEPROM."""

    name = BOARD_NAMES[data.get("name", "pocketbeagle")]

    version = data.get("version", "0000")
    week = f'{data.get("week", 0):02}'
    year = f'{data.get("year", 0):02}'
    uid = f'{data.get("id", 0):04}'
    serial_number = f"{week:02}{year:02}PSAS{uid:04}"

    value = HEADER + f"{name}{version}{serial_number}".encode()

    try:
        i2c.writeto(I2C_ADDR, EEPROM_ADDR + value)
    except Exception:
        return {"error": "failed to write board id to EEPROM"}

    # After receiving the write bytes, the EEPROM takes some time to write it to memory. The chip
    # with NAK during this time, so the datasheet suggests polling the chip via i2c until it acks a
    # command, at which point it can handle more commands. Here we attempt the initial command of
    # reading back what we just wrote, and when the chip ACKs the command, we can proceed with
    # the read.
    while True:
        try:
            i2c.writeto(I2C_ADDR, EEPROM_ADDR, False)
            i2c.readfrom(I2C_ADDR, len(value))
        except Exception:
            continue
        break

    try:
        i2c.writeto(I2C_ADDR, EEPROM_ADDR, False)
        value_readback = i2c.readfrom(I2C_ADDR, len(value))
    except Exception:
        return {"error": "failed to read board id back from EEPROM"}

    if value != value_readback:
        data = {"error": f"invalid readback; wrote {value.hex()} read {value_readback.hex()}"}
    else:
        led_timer.deinit()
        led.on()

    return data


i = 0
while True:
    sleep(1)
    results = poll.poll()

    req = ""
    while poll.poll(0):
        req += sys.stdin.read(1)

    if req == "":
        continue

    try:
        req_data = json.loads(req)
        res_data = eeprom_write(req_data)
        res = json.dumps(res_data)
    except Exception as e:
        res = json.dumps({"error": str(e)})
    print(res)
