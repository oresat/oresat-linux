Hardware
========

Octavo A8
---------

All OreSat Linux cards use a `Octavo A8`_, specficily the OSD3358-1G-BSM. 

A very simular SOC can be commonly found in Beagleboard's `PocketBeagle`_.

Key features of the Octavo A8:

    - A single core 1GHz AM335x ARM Cortex-A8 processor
    - 2x 200Mhz PRU (Programmable Real-time Unit)
    - A version with 1GB of RAM
    - 3D Graphics Accelerator
    - 6x UART buses
    - 2x CAN buses
    - 3x I2C buses
    - 2x SPI buses
    - 8 channel 12-bit ADC
    - 114x GPIO pins

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

    - `oresat-prucam-ar013x`_
    - `oresat-prucam-pirt1280`_


.. oresat repos
.. _oresat-prucam-ar013x: https://github.com/oresat/oresat-prucam-ar013x
.. _oresat-prucam-pirt1280: https://github.com/oresat/oresat-prucam-pirt1280

.. other links
.. _Octavo A8: https://octavosystems.com/octavo_products/osd335x-sm/
.. _PocketBeagle: https://beagleboard.org/pocket
.. _TI PRU-ICSS / PRU_ICSSG: http://software-dl.ti.com/processor-sdk-linux/esd/docs/latest/linux/Foundational_Components_PRU-ICSS_PRU_ICSSG.html
