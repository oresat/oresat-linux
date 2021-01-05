Update Archive Files
====================

OreSat Linux boards cane be updated thru the `oresat-linux-updater`_ daemon. It
is mostly a simple daemon wrapper for apt install local Debian package. It also 
supports running bash scipts, but all updates should be done with deb packages. 

Update archive files should be a .tar.gz archive file and follow OLM's file
transfer filename format with the keyword "update". The date field in the
filename will be used to determine the next file to use as the oldest file is
always used.

**Example, a update to the GPS board**::

   gps_update_21-10-03-14-30-27.tar.gz

An update archive should contant deb files, bash scripts, files used by 
bash script, and **always** include a instructions.txt file. If there is no
instructions.txt in the archive file, the and all following update will be 
deleted and the error message will be sent to OLM.

**Example contents of a update archive**::

    instructions.txt
    package1.deb
    package2.deb
    package3.deb
    bash_script1.sh
    bash_script2.sh
    bash_script3.sh
    bash_script2_external_file

instructions.txt
----------------

instruction.txt contatins a JSON string with with a list of instruction 
dictionaries with type and item fields. The instructions will be run in order.

The instruction types can be

- **install_pkg**: Install a deb package from the archive file.
- **remove_pkg**: Remove a deb package.
- **bash_script**: Run a bash script from the archive file.

**Example instructions.txt**::

    [
        {
            "type": "install_pkg",
            "item": "package1.deb"
        },
        {
            "type": "bash_script",
            "item": "bash_script1.sh"
        },
        {
            "type": "install_pkg",
            "item": "package2.deb"
        },
        {
            "type": "install_pkg",
            "item": "package3.deb"
        },
        {
            "type": "bash_script",
            "item": "bash_script2.sh"
        },
        {
            "type": "remove_pkg",
            "item": "package4"
        },
        {
            "type": "bash_script",
            "item": "bash_script3.sh"
        }
    ]

.. _oresat-linux-updater: https://github.com/oresat/oresat-linux-updater
