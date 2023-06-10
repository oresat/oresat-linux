#!/usr/bin/env python3
'''
This script writes the board ID to the EEPROM in the OSD3358 and reads it back afterwards.
'''

import sys
from argparse import ArgumentParser

from smbus2 import SMBus, i2c_msg

I2C_ADDR = 0x50
EEPROM_ADDR = b'\x00\x00'
BOARD_ID = b'\xaaU3\xeeA335PBGL00A21740GPB43424'

parser = ArgumentParser(description='Flash an Octavo A8 EEPROM')
parser.add_argument('-b', '--bus', type=int, default=2, help='I2C bus number; defualt is 2')
parser.add_argument('-r', '--read', action='store_true',
                    help='only read the current value and print it')
args = parser.parse_args()


board_id_hex = BOARD_ID.hex(sep=' ')

if args.read:
    print(f'Expected Board ID: {board_id_hex}')
else:
    write = i2c_msg.write(I2C_ADDR, EEPROM_ADDR + BOARD_ID)
    with SMBus(args.bus) as bus:
        bus.i2c_rdwr(write)
    print(f'Wrote Board ID: {board_id_hex}')

# After receiving the write bytes, the EEPROM takes some time to write it to memory. The chip with
# NAK during this time, so the datasheet suggests polling the chip via i2c until it acks a command,
# at which point it can handle more commands. Here we attempt the initial command of reading back
# what we just wrote (which is a write to 0x0000), and when the chip ACKs the command, we can
# proceed with the read
write = i2c_msg.write(I2C_ADDR, EEPROM_ADDR)
read = i2c_msg.read(I2C_ADDR, len(BOARD_ID))
while True:
    try:
        with SMBus(args.bus) as bus:
            bus.i2c_rdwr(write)
        with SMBus(args.bus) as bus:
            bus.i2c_rdwr(write, read)
        break
    except OSError:
        continue  # handle the OSError exception returned when we get a NAK


board_id_hex = bytes(read).hex(sep=' ')
print(f'Read Board ID: {board_id_hex}')

if bytes(read) != BOARD_ID:
    print('board id read from eeprom did not match expected board id')
    sys.exit(1)
