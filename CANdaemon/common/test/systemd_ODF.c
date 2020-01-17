// https://www.freedesktop.org/wiki/Software/systemd/dbus/


#include <systemd/sd-bus.h> // /usr/include/systemd/sd-bus.h
#include <stdio.h>

#define BUS_NAME  "org.freedesktop.systemd1"
#define INTERFACE_NAME  BUS_NAME".Manager"
#define OBJECT_PATH     "/org/freedesktop/systemd1"

/*
org.freedesktop.DBus
org.freedesktop.DBus.Properties 
/org/freedesktop/DBus
*/

int main() {
    sd_bus_error error = SD_BUS_ERROR_NULL;
    sd_bus *bus = NULL;
    int r;


    r = sd_bus_open_system(&bus);
    if(r < 0)
        printf("dbus error\n");

    r = sd_bus_call_method(bus,
                           BUS_NAME,
                           OBJECT_PATH,
                           INTERFACE_NAME,
                           "Reboot",
                           &error,
                           NULL,
                           NULL);
    if(r < 0)
        printf("dbus error\n");

    sd_bus_error_free(&error);
    sd_bus_unref(bus);
    return 0;
}
