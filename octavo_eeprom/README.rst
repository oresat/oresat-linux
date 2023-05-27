Octavo EEPROM
=============

This script will write a id value to the Octavo's EEPROM. When the Octavo is booting,
U-Boot will read this value in to figure out what device tree to load, before booting
up Linux.

This script will write the board id of a PocketBeagle to the EEPROM.

Id's can be found in the README of https://github.com/beagleboard/image-builder.

Requriements
------------

A `PocketBeagle`_.

Setup
-----

Clone the oresat-linux repo on the PocketBeagle.

    .. code-block::

       $ git clone https://github.com/oresat/oresat-linux
       $ cd oresat-linux/octavo_eeprom/

Writing EEPROM
--------------

- The Octavo's EEPROM write protection pin must be de-asserted.
- Power on the Octavo.
- Connect the PocketBeagle’s I2C-2 to the Octavo’s I2C-0.
- Run the Python script on the PocketBeagle.

    .. code-block::

       $ sudo ./i2c_program_board_id.py

.. _PocketBeagle: https://beagleboard.org/pocket
