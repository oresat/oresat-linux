Octavo EEPROM
=============

Requriements
------------

A `PocketBeagle`_.

Setup
-----

Clone the oresat-linux repo on the PocketBeagle.

    .. code-block::

       $ git clone https://github.com/oresat/oresat-linux
       $ cd oresat-linux/octavo-eeprom/

Writing EEPROM
--------------

- The Octavo's EEPROM write protection pin must be de-asserted.
- Power on the Octavo.
- Connect the PocketBeagle’s I2C-2 to the Octavo’s I2C-0.
- Run the Python script on the PocketBeagle.

    .. code-block::

       $ sudo ./i2c-program-board-id.py

.. _PocketBeagle: https://beagleboard.org/pocket
