#!/usr/bin/env python3


import sys, os
import updater


PID_FILE = '/run/oresat-linux-updater.pid'
DBUS_INTERFACE_NAME = "org.OreSat.LinuxUpdater"


class LinuxUpdaterDbus(object):
    def __init__(self):
        self.dbus = """
        <node>
            <interface name="org.OreSat.LinuxUpdater">
                <method name='AddArchiveFile'>
                    <arg type='s' name='file_path' direction='in'/>
                    <arg type='b' name='output' direction='out'/>
                </method>
                <method name='StartUpdate'>
                    <arg type='b' name='output' direction='out'/>
                </method>
                <method name='ForceUpdate'>
                    <arg type='s' name='file_path' direction='in'/>
                    <arg type='b' name='output' direction='out'/>
                </method>
                <method name='GetAptListOutput'>
                    <arg type='b' name='output' direction='out'/>
                </method>
                <property name="CurrentState" type="d" access="read"/>
                <property name="CurrentArchiveFile" type="s" access="read"/>
                <property name="AvailableArchiveFiles" type="d" access="read"/>
            </interface>
        </node>
        """
        self._updater = LinuxUpdater()


        def __del__(self):
            self._updater.quit()


        @property
    def CurrentState(self):
        return self._updater.current_state


    @property
    def CurrentArchiveFile(self):
        return self._updater.current_archive_file


    @property
    def AvailableArchiveFiles(self):
        return self._updater.available_archive_file


    def AddArchiveFile(self, file_path):
        return self._updater.add_archive_file(file_path)


    def StartUpdate(self):
        return self._updater.start_update()


    def GetAptListOutput(self):
        return self._updater.get_apt_list_output()


if __name__ == "__main__":
    # Check for a pidfile to see if the daemon is already running
    try:
        with open(PID_FILE,'r') as pf:

            pid = int(pf.read().strip())
    except IOError:
        pid = None

    if pid:
        sys.stderr.write("pid file {0} already exist.\n".format(PID_FILE))
        sys.exit(1)

    try: 
        pid = os.fork() 
        if pid > 0:
            # exit from parent
            sys.exit(0) 
    except OSError as err: 
        sys.stderr.write('fork failed: {0}\n'.format(err))
        sys.exit(1)

    # decouple from parent environment
    os.chdir('/') 
    os.setsid() 
    os.umask(0) 

    # redirect standard file descriptors
    sys.stdout.flush()
    sys.stderr.flush()
    si = open(os.devnull, 'r')
    so = open(os.devnull, 'a+')
    se = open(os.devnull, 'a+')

    os.dup2(si.fileno(), sys.stdin.fileno())
    os.dup2(so.fileno(), sys.stdout.fileno())
    os.dup2(se.fileno(), sys.stderr.fileno())

    pid = str(os.getpid())
    with open(PID_FILE,'w+') as f:
        f.write(pid + '\n')

    # make updater
    updater = LinuxUpdaterDbus()
    
    # set up dbus wrapper
    bus = SystemBus()
    bus.publish(DBUS_INTERFACE_NAME, updater)

    # start dbus wrapper
    loop = GLib.MainLoop()
    try:
        loop.run()
    except KeyboardInterrupt as e:
        loop.quit()

    # remove pid file
    os.remove(self.delpid)
