import dbus

sysbus = dbus.SystemBus() # connect
systemd1 = sysbus.get_object('org.freedesktop.systemd1', '/org/freedesktop/systemd1') # don't need in c

# busctl introspect org.freedesktop.systemd1.Manager /org/freedesktop/systemd1/Manager
manager = dbus.Interface(systemd1, 'org.freedesktop.systemd1.Manager') 
service = sysbus.get_object('org.freedesktop.systemd1', object_path=manager.GetUnit('networking.service')) # GetUnit dbus method

# busctl introspect org.freedesktop.DBus /org/freedesktop/DBus
interface = dbus.Interface(service, dbus_interface='org.freedesktop.DBus.Properties')
print(interface.Get('org.freedesktop.systemd1.Unit', 'ActiveState'))
