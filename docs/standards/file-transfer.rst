File Transfer Filename Format
=============================

`oresat-linux-manager`_ support file transfer, so file can be written and read
from Linux boards. All file transfer to and from a Linux board must follow a 
filename format.

**File Format**::

   board_keyword_datetime.fileformat

**Format Rules:**

- all lowercase
- **board**: The board name / acronym; ie gps, st (aka star tracker), live, or
  cfc.
  This will allow the `C3`_ to know which board to write the file to.
- **keyword**: A **unique** key for the service for OLM to use when the file is
  being deliver to it; ie If a file has the "update" keyword, OLM will move it
  to location defined by the OreSat Linux Updater app.
- **datetime**: The interger Unix time the file was made.
- **.fileformat**: Optional. Can be anything.
- '_' is only used to seperate the **board**, **keyword**, and **datetime**
  fields and nowhere else.

**Example, a update to the GPS board**::

   gps_update_161272609.tar.xz


.. OreSat repos
.. _oresat-linux-manager: https://github.com/oresat/oresat-linux-manager
.. _oresat-linux-updater: https://github.com/oresat/oresat-linux-updater
.. _C3: https://github.com/oresat/oresat-c3

.. Other repos
.. _CANopenNode: https://github.com/CANopenNode/CANopenNode
.. _sd-bus: https://github.com/systemd/systemd/blob/master/src/systemd/sd-bus
.. _sdbus-cpp: https://github.com/Kistler-Group/sdbus-cpp/
.. _pydbus: https://github.com/LEW21/pydbus

.. Other links
.. _CAN: https://en.wikipedia.org/wiki/CAN_bus
.. _CANopen: https://en.wikipedia.org/wiki/CANopen
.. _DBus: https://en.wikipedia.org/wiki/D-Bus
.. _DKMS: https://github.com/dell/dkms
.. _beagleboard: https://beagleboard.org/
.. _freedesktop dbus bindings: https://www.freedesktop.org/wiki/Software/DBusBindings/
