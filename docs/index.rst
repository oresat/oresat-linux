Welcome to OreSat Linux's documentation!
========================================

Documenation for the embedded Linux boards on OreSat.

The goal of this documentation is to provide a overview of the OreSat Linux
enviroment on OreSat Linux boards, a high level overview of each OreSat Linux
board, and provide any helpful resources/guides for development on OreSat Linux
boards.

.. warning::
    This is still a work in progress.

.. note:: This is **not** the documentation for any of the Cortex-M0 or 
   Cortex-M4 boards running ChibiOS or the ground station software, see 
   `OreSat Firmware`_ or `UniClOGS Software`_ respectively.

What is OreSat?
---------------

OreSat is a open source CubeSat from Oregon by PSAS (Portland State Aerospace
Society).

.. note::

    If you are looking for more info on OreSat, go checkout  
    `OreSat Website`_ and `OreSat GitHub`_

Why Linux?
----------

Some of subsystems on OreSat need extra computing power, so they’re given
hardware that can run Linux.

Linux is Open Source.

This also remove the restriction of baremetal, allowing software to be written
in languages like Python, Java, Haskell, etc as needed.

Glossary
--------

.. toctree::
    :maxdepth: 2

    glossary

Getting Started
---------------

.. toctree::
    :maxdepth: 2

    getting_started

General Design
--------------

.. toctree::
    :maxdepth: 2
    
    general_design/index

Linux Images
------------

.. toctree::
    :maxdepth: 2
    
    images/index

Guides
------

.. toctree::
    :maxdepth: 2
    
    guides/index

Git Repositories
----------------

.. toctree::
    :maxdepth: 2
    
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
.. _UniClOGS Software: https://uniclogs-software.readthedocs.io/en/latest/

