Welcome to OreSat Linux's documentation!
========================================

Documentation for the embedded Linux cards on OreSat.

The goal of this documentation is to provide a overview of the OreSat Linux
environment on OreSat Linux cards, a high level overview of each OreSat Linux
card, and provide any helpful resources/guides for development on OreSat Linux
cards.

.. warning:: This is still a work in progress.

.. note:: This is **not** the documentation for any of the Cortex-M0 or 
   Cortex-M4 cards running ChibiOS or the ground station software, see 
   `OreSat Firmware`_ or `UniClOGS Docs`_ respectively.

What is OreSat?
---------------

OreSat is a open source CubeSat from Oregon by PSAS (Portland State Aerospace
Society).

.. note:: If you are looking for more info on OreSat, go checkout  
    `OreSat Website`_ and `OreSat GitHub`_

Why Linux?
----------

Some of subsystems on OreSat need extra computing power, so they’re given
hardware that can run Linux.

Linux is Open Source.

This also remove the restriction of baremetal and minimum OSes, allowing
software to be written in languages like Python, Java, Haskell, etc as needed.

Glossary
--------

.. toctree::
    :maxdepth: 2

    glossary
    getting_started
    general_design/index
    images/index
    guides/index
    repos

OreSat Linux Links
-------------------
- All Oresat Linux Packages can be found at https://packages.oresat.org/
- All Oresat Linux images can be found at https://images.oresat.org/


Indices and Tables
==================

* :ref:`genindex`
* :ref:`modindex`
* :ref:`search`


.. _OreSat Website: https://www.oresat.org/
.. _OreSat GitHub: https://github.com/oresat

.. _OreSat Firmware: https://github.com/oresat/oresat-firmware
.. _UniClOGS Docs: https://uniclogs-docs.readthedocs.io/en/latest/

