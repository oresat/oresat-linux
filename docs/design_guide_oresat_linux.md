# OreSat Linux design guide
A design guide / notes / standards with reasonings for OreSat Linux.


## Operating System
- Use Debian kernel 4.19. An image can be found [here](https://elinux.org/Beagleboard:BeagleBoneBlack_Debian#Debian_Buster_Console_Snapshot).
    - *Support for pru.*
    - *Beagleboard made the image, they added a lot on nice tools (uboot overlay scripts / pru kernel module).*


## General system
- Use systemd-networkd for networking.
    - *Built into systemd.*
    - *Easy to configure networks to enable on boot up (only really need CAN).*


## OreSat board's main process standards
A guide / standards with reasonings for main process on all OreSat Linux boards.
### Directories workspace
- Working dir is `/`.
    - *Root process.*
    - *Standard practice.*
    - *No user directory*
- Use `/var/cache/` as needed, but manage it if needed. Make a directory in for that app.
    - *No user directories.*
    - *Standard practice.*
    - *oresat linux manager will not manage an app's memory.* 
- Use `/tmp/` as needed, but manage it if needed. 
    - *No user directories.*
    - *Standard practice.*
- Install app source binary to `/usr/bin/`.
    - *Standard practice.*
- Install app source python code to `/opt/`. 
    - *No user directories.*

### Daemonize
- Daemonize it (forked and long running background process). Has a systemd service file. Install service file to `/lib/systemd/systemd/`
    - *There is no user to start a process when the board is powered on.*
    - *The oresat linux manager can use systemd to control the daemon.*
- Follow deamon service file naming of `oresat-boardd.service`, ie `oresat-lived.service`.
- The oresat linux manager and Linux updater are the only enabled OreSat daemons. The main process daemon will **not** be enabled.
    - *The oresat linux manager needs to enable due to it being the main way to control the Linux board over the CANbus.*
    - *The updaters need to be enable to resume updates or reverts after power shutdowns incase packages are in a broken state (a broken oresat linux manager is a dead board).*
    - *The C3 card will tell the a Linux board's oresat linux manager to turn on the main process when the C3 card wants it up.*

### DBus
- Use DBus
    - *A lot of lanagues have support for dbus bindings, see [freedesktop dbus bindings] for a list.*
    - *The oresat linux manager acts as the frontend and interface for the CANbus, therefor the main projects can be implemented in mostly any programing language and not have to worry about finding / making a CAN library following [CANopen] 301 specifications for that specific language.*
- Only use system dbus **not** session dbus (aka user dbus). Install conf files to `/usr/share/dbus-1/system.d/`.
    - *No users, so no user dbus space.*
- Follow dbus config file naming of `org.OreSat.LinuxBoard`, ie `org.OreSat.StarTracker`.
- Use DBus methods to controlling and commanding daemons. 
- Use signals for async reply to dbus methods. (The method sets a flag varible on the server and returns. Then the server process the flag, do whaterver is needed, and uses a dbus signal to responed.)
    - *Dbus Methods should be very quick function calls, oresat linux manager only has one thread per dbus interface.*
- Use Dbus properties for the daemons public data; ie GPS would have a dbus property for the state vector of the satellite (postion, velocity, time stamp) and another property for its current state. 
    - *The oresat linux manager acts as the frontend and interface for the CANbus.*
- Use Dbus properties with the "emit when changed signal flag" set. 
    - *This is for oresat linux manager to know when to read properties and not just spinning reading every x seconds.*
- Have a clean cache dbus method.
    - *candemon will check systemd memory and may need to clear more space*.

### Specific DBus Library
- For C or C++ use [sd-bus].
    - *Built into systemd. Why use a different library?*
- For Python3 use [pydbus]. Can install it by just `apt install python3-python`.
    - *A good python dbus library.*
- For others?
    - *We mostly tend use C, C++, and Python in PSAS.*
    - *A list of supported dbus library for other langauges can be found here: [freedesktop dbus bindings]*

### Debian package
- Make it a debian package. 
    - *We use debian on the satellite. Makes updating the app easy.*


[CANopen]:https://en.wikipedia.org/wiki/CANopen
[sd-bus]:https://github.com/systemd/systemd/blob/master/src/systemd/sd-bus.h
[pydbus]:https://github.com/LEW21/pydbus
[freedesktop dbus bindings]:https://www.freedesktop.org/wiki/Software/DBusBindings/
