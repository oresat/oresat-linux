#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <systemd/sd-bus.h>


#define INTERFACE_NAME  "org.example.project.oresat"
#define BUS_NAME        INTERFACE_NAME
#define OBJECT_PATH     "/org/example/project/oresat"


static int dbus_error(char* err, int r) {
    fprintf(stderr, "%s %s\n", err, strerror(-r));
    exit(0);
    return 1;
}


int main(int argc, char *argv[]) {
    int r;
    sd_bus_error err = SD_BUS_ERROR_NULL;
    sd_bus_message *mess = NULL;
    sd_bus *bus = NULL;
    int16_t posX = 0;

    /* Connect to the bus */
    r = sd_bus_open_system(&bus);
    if (r < 0)
        dbus_error("Failed to connect to system bus:", r);

    r = sd_bus_get_property(bus,
                            BUS_NAME,
                            OBJECT_PATH,
                            INTERFACE_NAME,
                            "PositionY",
                            &err,
                            &mess,
                            "n");
    if(r < 0) {
        fprintf(stderr, "%s\n", strerror(sd_bus_error_get_errno(&err)));
        dbus_error("Get property failed:", r);
    }

    r = sd_bus_message_read(mess, "n", &posX);
    if(r < 0)
        dbus_error("Read message failed:", r);

    printf("%d\n", posX);

    sd_bus_error_free(&err);
    sd_bus_message_unref(mess);
    sd_bus_unref(bus);
    return 1;
}
