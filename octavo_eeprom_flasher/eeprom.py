#!/usr/bin/env python3
"""Writes the board ID to the EEPROM in the OSD3358 or reads it back."""

import re
from argparse import ArgumentParser, Namespace, RawDescriptionHelpFormatter
from dataclasses import dataclass
from datetime import UTC, datetime
from struct import calcsize, unpack
from textwrap import dedent
from typing import ClassVar

try:
    from periphery import I2C, I2CError
except ImportError:
    raise SystemExit("Error: I2C Dependency not found, consider apt install python3-periphery")

# FIXME: Move to oresat-configs
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


@dataclass
class Identifier:
    '''Represents the identifier written to EEPROM.

    This follows the format of the EEPROM on the pocketbeagle and at least the
    name is required by u-boot to be set to a known value to boot.  Older
    versions of u-boot only accepted the pocketbeagle and beagleboneblack
    options but the we now maintain a patchset that adds the OreSat specific
    names to u-boot for the newer version.

    The format is:
     - 4 byte specific header
     - 8 bytes ASCII name
     - 2 bytes each ASCII hex major and minor versions
     - 2 bytes each ASCII decimal week and year
     - 4 bytes ASCII PSAS
     - 4 bytes ASCII decimal board number.
    '''

    name: str
    major: int
    minor: int
    number: int
    year: int
    week: int
    HEADER: ClassVar[bytes] = b"\xaa\x55\x33\xee"
    idformat: ClassVar[str] = '4s8s2s2s2s2s4s4s'

    def encode(self) -> bytes:
        '''Encode this identifier to a byte string.

        Returns
        -------
        This object represented as a bytes string
        '''
        name = BOARD_NAMES[self.name]
        version = f'{self.major:02X}{self.minor:02X}'
        serial = f"{self.week:02}{self.year:02}PSAS{self.number:04}"
        raw = self.HEADER + f"{name}{version}{serial}".encode()
        if len(raw) != calcsize(self.idformat):
            raise ValueError(f"Invalid encode message length {len(raw)}: {raw!r}")
        return raw

    @classmethod
    def decode(cls, raw: bytes) -> "Identifier":
        '''Decode a bytes into this class.

        Parameters
        ----------
        raw:
            The bytes that are expected to match this identifier

        Raises
        ------
        ValueError:
            If the raw bytes cannot be decoded.
        '''
        header, name, major, minor, week, year, psas, number = unpack(cls.idformat, raw)
        if header != cls.HEADER:
            raise ValueError(f"Unable to decode raw bytes, header incorrect: {raw!r}")
        if psas.decode() != 'PSAS':
            raise ValueError(f"Unable to decode raw bytes, PSAS missing: {raw!r}")
        return cls(
            BOARD_NAMES_REV.get(name.decode(), "unknown"),
            int(major.decode(), 16),
            int(minor.decode(), 16),
            int(number.decode()),
            int(year.decode()),
            int(week.decode()),
        )


class Eeprom:
    '''Utils for reading and writing to a Microchip 24AA32A EEPROM.

    This is the EEPROM built into the Octavo OSD3358, the SOC we use for OreSat cards. In addition
    to connecting I2C lines, ensure the write protect pin is grounded if writing.
    Find the datasheet here: https://www.microchip.com/en-us/product/24AA32A
    '''

    EEPROM_ADDR = 0x50
    MEMORY_ADDR = b"\x00\x00"

    def __init__(self, i2c: I2C) -> None:
        '''Create an Eeprom instance.

        Parameters
        ----------
        i2c:
            An initialized I2C instance that the EEPROM is on
        '''
        self.i2c = i2c

    def read(self) -> bytes:
        '''Read the identifier length of bytes from EEPROM address 0.

        Returns
        -------
        The bytes in the location the OreSat ID would be,
        '''
        body = I2C.Message(bytes(calcsize(Identifier.idformat)), read=True)
        self.i2c.transfer(self.EEPROM_ADDR, [I2C.Message(self.MEMORY_ADDR), body])
        return bytes(body.data)

    def write(self, raw: bytes) -> None:
        """Write up to 32 bytes to EEPROM.

        The data is written to address 0 (MEMORY_ADDR) as a single page write and then the
        current address is reset. Ensure that the write protect pin is grounded for writes to work

        Parameters
        ----------
        raw:
            The raw bytes to write, up to 32 in length.
        """
        if len(raw) > 32:
            raise ValueError(f"Too many bytes to write: {len(raw)} greater than max 32")
        self.i2c.transfer(self.EEPROM_ADDR, [I2C.Message(self.MEMORY_ADDR + raw)])
        # After receiving the write bytes, the EEPROM takes some time to write
        # it to memory. The chip responds with NAK during this time so the
        # datasheet suggests polling the chip via i2c until it acks a command,
        # at which point it can handle more commands. Here we attempt to reset
        # the current address and when the chip ACKs the command, we can
        # proceed.

        # While trying to determine which exception this would cause I've not
        # actually encountered this condition though. If we hit this in
        # practice fill out the appropriate section below and uncomment the loop
        # while True:
        # try:
        self.i2c.transfer(self.EEPROM_ADDR, [I2C.Message(self.MEMORY_ADDR)])
        # except ???:
        #    continue
        # break


def do_read(args: Namespace) -> None:
    '''Read the EEPROM ID and print its value.

    Parameters
    ----------
    args:
        An argparse namespace containing i2c - the path to the i2c device that the EEPROM is on
    '''
    try:
        i2c = I2C(args.I2C_PATH)
    except I2CError as e:
        raise SystemExit(f"While opening I2C device '{args.I2C_PATH}': {e}") from None
    raw = Eeprom(i2c).read()
    print(raw)
    print(Identifier.decode(raw))


def do_write(args: Namespace) -> None:
    '''Write the EEPROM ID to the card.

    This also validates the arguments and will exit with an error code if the arguments are invalid

    Parameters
    ----------
    args:
        An argparse namespace containing:
        - i2c - the path to the i2c device that the EEPROM is on
        - card - one of the keys of BOARD_NAMES
        - revision - the card revision in x.y format
        - number - the board number
        - date - optional, in YY-WW format
    '''
    if args.CARD not in BOARD_NAMES:
        raise SystemExit(f"Error: card '{args.CARD}' not one of: {' '.join(BOARD_NAMES)}")

    if revmatch := re.match(r"^(\d+)\.(\d+)$", args.REVISION):
        major = int(revmatch.group(1))
        minor = int(revmatch.group(2))
    else:
        raise SystemExit(f"Error: Revision '{args.REVISION}' not in x.y format")
    if not 0 <= major <= 0xFF:
        raise SystemExit(f'Error: Revision major number ({major}) must be between 0 and 255')
    if not 0 <= minor <= 0xFF:
        raise SystemExit(f'Error: Revision minor number ({minor}) must be between 0 and 255')

    if not args.date:
        year, week, _ = datetime.now(UTC).isocalendar()
        year %= 100
    elif datematch := re.match(r"^(\d\d)-(\d\d)$", args.date):
        year = int(datematch.group(1))
        week = int(datematch.group(2))
    else:
        raise SystemExit(f"Error: Date '{args.date}' not in YY-WW format")

    data = Identifier(args.CARD, major, minor, args.NUMBER, year, week).encode()
    try:
        i2c = I2C(args.I2C_PATH)
    except I2CError as e:
        raise SystemExit(f"While opening I2C device '{args.I2C_PATH}': {e}") from None

    eeprom = Eeprom(i2c)
    eeprom.write(data)
    readback = eeprom.read()

    if data != readback:
        raise ValueError(f"invalid readback; wrote {data.hex()} read {readback.hex()}")
    print(readback)


if __name__ == '__main__':
    parser = ArgumentParser(
        formatter_class=RawDescriptionHelpFormatter,
        description=dedent('''
            Utility for interacting with the EEPROM on OreSat Octavo cards using a usb-i2c adaptor.
            See the associated README for how to attach to the card.

            Use `i2cdetect -l` from the i2c-tools package to find the /dev/i2c-x for your adaptor.
            **Note**: Picking the wrong I2C device may damage your own host machine. 

             See `eeprom.py read --help` or `eeprom.py write --help` for more.
        ''')
    )
    subparsers = parser.add_subparsers(
        required=True,
        help='read or write the OreSat EEPROM ID to an Octavo card.',
    )
    reader = subparsers.add_parser('read', help='I2C_PATH')
    reader.add_argument("I2C_PATH", help="I2C bus path, typically /dev/i2c-x")
    reader.set_defaults(func=do_read)

    writer = subparsers.add_parser('write', help='I2C_PATH CARD REVISION NUMBER [-d DATE]')
    writer.add_argument("I2C_PATH", help="I2C bus path, typically /dev/i2c-x")
    # FIXME: get board names + aliases from oresat-configs
    writer.add_argument("CARD", help=f"Card type, one of: {' '.join(BOARD_NAMES)}")
    writer.add_argument("REVISION", help="Card revision in x.y format")
    writer.add_argument(
        "NUMBER", type=int, help="This card's board number, typically written on the PCB"
    )
    writer.add_argument(
        '-d',
        '--date',
        help="Year and week the card was made in YY-WW format. Defaults to current date",
    )
    writer.set_defaults(func=do_write)
    args = parser.parse_args()
    args.func(args)
