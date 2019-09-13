#include <stdio.h>
#include <stdlib.h>
#include <systemd/sd-bus.h>


#define INTERFACE_NAME  "org.example.project.oresat"
#define BUS_NAME        INTERFACE_NAME
#define OBJECT_PATH     "/org/example/project/oresat"
//#define USER_DBUS       1 // comment out if system dbus is wanted


static int dbus_error(char* err, int r) {
    fprintf(stderr, "%s %s\n", err, strerror(-r));
    exit(0);
    return 1;
}


int main(int argc, char *argv[]) {
    sd_bus *bus = NULL;
    int r;

    /* Connect to the bus */
#ifdef USER_DBUS
    r = sd_bus_open_user(&bus);
    if(r < 0)
        dbus_error("Failed to connect to user bus:", r);
#else
    r = sd_bus_open_system(&bus);
    if (r < 0)
        dbus_error("Failed to connect to system bus:", r);
#endif

    sd_bus_error err;

    r = sd_bus_set_property(bus,
                         BUS_NAME,
                         OBJECT_PATH,
                         INTERFACE_NAME,
                         "TestProp",
                         &err,
                         "d",
                         10.0);
    if(r < 0)
        dbus_error("Set property  error:", r);

    sd_bus_message *mess = NULL;
    double test_double = 0.0;

    for(;;) {
        r = sd_bus_get_property(bus,
                            BUS_NAME,
                            OBJECT_PATH,
                            INTERFACE_NAME,
                            "TestProp",
                            &err,
                            &mess,
                            "d");

        if(r < 0)
            dbus_error("Get property failed:", r);

        r = sd_bus_message_read(mess, "d", &test_double);
        if(r < 0)
            dbus_error("Read message failed:", r);
        printf("%f\n", test_double);
    }

    sd_bus_unref(bus);
    return 1;
}
