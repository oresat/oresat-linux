#!/usr/bin/env python3


from updater import LinuxUpdater


DBUS_INTERFACE_NAME = "org.OreSat.LinuxUpdater"


class LinuxUpdaterDbus(object):
    dbus = """
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
            <property name="CurrentState" type="d" access="read">
                <annotation name="org.freedesktop.DBus.Property.EmitsChangedSignal" value="true"/>
            </property>
            <property name="CurrentArchiveFile" type="s" access="read">
                <annotation name="org.freedesktop.DBus.Property.EmitsChangedSignal" value="true"/>
            </property>
            <property name="AvailableArchiveFiles" type="d" access="read">
                <annotation name="org.freedesktop.DBus.Property.EmitsChangedSignal" value="true"/>
            </property>
        </interface>
    </node>
    """


    def __init__(self):
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
        return self._updater.available_archive_files


    def AddArchiveFile(self, file_path):
        return self._updater.add_archive_file(file_path)


    def StartUpdate(self):
        return self._updater.start_update()


    def GetAptListOutput(self):
        return self._updater.get_apt_list_output()

    def quit():
        return self._updater.quit()


