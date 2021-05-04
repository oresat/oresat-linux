Hardware
========

Octavo A8
---------

All Linux boards use a `Octavo A8`_. It is commonly found in Beagleboard's
`PocketBeagle`_.

Key features of the Octavo A8:

    - A single core 1GHz AM335x ARM Cortex-A8 processor
    - 2x 200Mhz PRU (Programable Real-time Unit)
    - A version with 1GB of RAM
    - 3D Graphics Accelerator
    - Support for UART, CAN, I2C, & SPI 

See https://octavosystems.com/octavo_products/osd335x-sm/ for a full list of
features of the Octavo A8.


PRU (Programmable Real-time Unit)
*********************************

A PRU is a microcontroller that shares pins and other resource with the core
processor allowing for custom interface to other hardware like cameras. The 
AM335x has two PRUs. For OreSat an idea is use them to get images from camera
sensors.

All information and guides about PRU can be found at: `TI PRU-ICSS / PRU_ICSSG`_

OreSat project(s) using PRUs: 

    - `oresat-linux-prucam`_

.. oresat repos
.. _oresat-linux-prucam: https://github.com/oresat/oresat-linux-prucam

.. other links
.. _Octavo A8: https://octavosystems.com/octavo_products/osd335x-sm/
.. _PocketBeagle: https://beagleboard.org/pocket
.. _TI PRU-ICSS / PRU_ICSSG: http://software-dl.ti.com/processor-sdk-linux/esd/docs/latest/linux/Foundational_Components_PRU-ICSS_PRU_ICSSG.html
