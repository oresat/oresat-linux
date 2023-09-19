"""
This is MicroPython!!

This script writes the board ID to the EEPROM in the OSD3358 and reads it back afterwards.
"""

from machine import I2C, Pin, Timer
from time import sleep

I2C_ADDR = 0x50
EEPROM_ADDR = b"\x00\x00"
BOARD_ID = b"\xaaU3\xeeA335PBGL00A21740GPB43424"  # BeagleBoard Pocketbeagle id

led = Pin(25, Pin.OUT)
i2c = I2C(0, scl=Pin(17), sda=Pin(16), freq=400_000)


timer = Timer(freq=1000, callback=(lambda t: led.toggle()))
timer.start()

while True:
    sleep(0.1)

    try:
        i2c.writeto_mem(I2C_ADDR, EEPROM_ADDR, BOARD_ID, addrsize=16)
    except Exception:
        print("ERROR: failed to write board id to EEPROM")
        continue

    # After receiving the write bytes, the EEPROM takes some time to write it to memory. The chip
    # with NAK during this time, so the datasheet suggests polling the chip via i2c until it acks a
    # command, at which point it can handle more commands. Here we attempt the initial command of
    # reading back what we just wrote, and when the chip ACKs the command, we can proceed with
    # the read.
    while True:
        try:
            i2c.writeto(I2C_ADDR, EEPROM_ADDR, stop=True)
        except Exception:
            break

    try:
        board_id_readback = i2c.readfrom_mem(I2C_ADDR, EEPROM_ADDR, len(BOARD_ID), addrsize=16)
    except Exception:
        print("ERROR: failed to read board id back from EEPROM")
        continue

    if BOARD_ID != board_id_readback:
        print(f"ERROR: invalid readback; wrote {BOARD_ID.hex()} read {board_id_readback.hex()}")
    else:
        timer.stop()
        break
