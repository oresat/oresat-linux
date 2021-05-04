.. _glossary:

=======================
 Glossary of Terms Used
=======================

.. glossary::
    :sorted:

    CubeSat
        A small satellite is made up of multiples of 10cm × 10cm × 10cm cubic
        units.

    OreSat
        PSAS's open source CubeSat. See https://www.oresat.org/

    PSAS
        Portland State Aerosapce Society. A student aerospace group at Portland
        State University. See https://www.pdxaerospace.org/

    D-Bus
        Desktop Bus. Inter-process communication system common on Linux.
        See https://www.freedesktop.org/wiki/Software/dbus/

    DKMS
        Dynamic Kernel Module Support. It is used to automatically reload out of
        tree Linux kernel modules when the kernel is updated.

    CFC
        Cirrus Flux Camera. One of OreSat payloads and a Linux board.

    SDR
        Software Define Radio. Radio communications that are traditionally
        implemented in hardware are instead implemented in software.

    C3
        Command, communication, and control board. It is the flight computer for
        OreSat. See https://github.com/oresat/oresat-c3

    Daemon
        Long running, background process on Linux. AKA a Linux service.

    CAN
        Control Area Network. A message bus for embedded systems.

    CANopen
        A communication protocol and device profile specification for a CAN 
        bus defined by CAN in Automation. More info at https://can-cia.org/

    OLM
        OreSat Linux Manager. The front end daemon for all OreSat Linux boards.
        It converts CANopen messages into D-Bus messages and vice versa. See
        https://github.com/oresat/oresat-linux-manager
