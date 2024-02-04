"""
This is MicroPython!

This script writes the board ID to the EEPROM in the OSD3358 and reads it back afterwards.
"""

import json
import select
import sys
from machine import I2C, Pin, Timer

I2C_ADDR = 0x50
EEPROM_ADDR = b"\x00\x00"
HEADER = b"\xAA\x55\x33\xEE"

BOARD_NAMES = {
    "pocketbeagle": "A335PBGL",
    "c3": "A335OSC3",
    "star-tracker": "A335OSST",
    "gps": "A335OGPS",
    "dxwifi": "A335ODWF",
    "cfc": "A335OCFC",
}

led = Pin(25, Pin.OUT)
led.off()
timer = Timer(mode=Timer.PERIODIC, freq=1, callback=lambda t: led.toggle())

i2c = I2C(0, scl=Pin(17), sda=Pin(16), freq=400_000)

poll = select.poll()
poll.register(sys.stdin, select.POLLIN)


def eeprom_read() -> dict:
    try:
        i2c.writeto(I2C_ADDR, EEPROM_ADDR, False)
        raw = i2c.readfrom(I2C_ADDR, 28)
    except Exception:
        return {"error": "failed to read data from EEPROM"}

    data = raw.decode()
    name = BOARD_NAMES.values().index(data[4:12])
    values = {
        "direction": "read",
        "name": list(BOARD_NAMES.keys())[name],
        "major": data[12:14],
        "minor": data[14:16],
        "week": data[16:18],
        "year": data[18:20],
        "id": data[24:28],
    }
    return values


def eeprom_write(data: dict) -> dict:
    """Write the data to EEPROM."""

    name = BOARD_NAMES[data.get("name", "pocketbeagle")]

    major = f'{data.get("version_major", 0)}:02'
    minor = f'{data.get("version_minor", 0)}:02'
    version = f"{major}{minor}"

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
        timer.deinit()
        led.on()
        data["direction"] = "read"

    return data


while True:
    results = poll.poll(1)
    if results:
        raw = sys.stdin.readline().strip()
        try:
            data = json.loads(raw.decode())
            direction = data.get("direction", "read")
            if direction == "read":
                ret_data = eeprom_read()
            else:
                ret_data = eeprom_write(data)
            ret = json.dumps(ret_data)
        except Exception as e:
            ret = json.dumps({"error": str(e)})
        print(ret)
    else:
        continue
