Octavo EEPROM Flasher
=====================

This script will write a id value to the Octavo's EEPROM. When the Octavo is booting,
U-Boot will read the value from EEPROM in to figure out what device tree to load,
before booting up Linux.

This script will write the board id of a PocketBeagle to the EEPROM.

Id's can be found in the README of https://github.com/beagleboard/image-builder.

Requriements
------------

A `PocketBeagle`_ or any device with an I2C bus.

Setup
-----

Clone the oresat-linux repo on the PocketBeagle.

.. code-block::

   $ git clone https://github.com/oresat/oresat-linux
   $ cd oresat-linux/octavo_eeprom_flasher

Install dependencies

.. code-block::

   $ sudo pip3 install -r requirements.txt

Writing EEPROM
--------------

- Connect the PocketBeagle’s I2C-2 to the Octavo’s I2C-0 using a Debug Board.

 .. warning:: Connection point to the Octavo's I2C-0 may change to be directly on card and not
    debug board with future OreSat cards.

 .. image:: ../static/oresat_debug_board.jpg

- Power on the Octavo.
- Run the Python script on the PocketBeagle (change the bus argument as needed).

 .. code-block::

   $ sudo ./eeprom_flasher.py --bus 2

 .. note:: The ``eeprom_flasher.py`` script has help message, just run the
    script with either the ``-h`` or ``--help`` flag to see it.

.. _PocketBeagle: https://beagleboard.org/pocket
