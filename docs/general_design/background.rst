Background Info
===============

CAN and CANopen
---------------

The communication bus on OreSat is a CAN bus and the CANopen protocol is used
on top of it. All OreSat Linux cards use `OLAF (OreSat Linux App Framework)`_
which is build around the `pythonic CANopen`_ library. All firmware cards
on OreSat use the `CANopenNode`_ stack to handle the CANopen protocols.

Helpful links:

- CAN bus overview:  https://en.wikipedia.org/wiki/CAN_bus
- CANopen protocol overview: https://www.can-cia.org/canopen
- All CANopen protocol specifications: https://can-cia.org/groups/specifications/

C3 (Command, communication, and control card)
----------------------------------------------

The `C3`_ is the flight computer for OreSat and handle all Earth to satellite
communications. It can turn other cards on or off thru the OPD (OreSat Power
Domain) and is CANopen Network Manager.

.. _C3: https://github.com/oresat/oresat-c3
.. _CANopenNode: https://github.com/CANopenNode/CANopenNode
.. _pythonic CANopen: https://github.com/christiansandberg/canopen
.. _OLAF (OreSat Linux App Framework): https://github.com/oresat/oresat-olaf
