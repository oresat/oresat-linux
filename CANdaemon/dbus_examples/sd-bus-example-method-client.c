#include <stdio.h>
#include <stdlib.h>
#include <systemd/sd-bus.h>


#define INTERFACE_NAME  "org.example.project.oresat"
#define BUS_NAME        INTERFACE_NAME
#define OBJECT_PATH     "/org/example/project/oresat"
#define USER_DBUS       1 // comment out if system dbus is wanted


static int dbus_error(char* err, int r) {
    fprintf(stderr, "%s %s\n", err, strerror(-r));
    exit(0);
    return 1;
}


int main(int argc, char *argv[]) {
    sd_bus_error error = SD_BUS_ERROR_NULL;
    sd_bus_message *m = NULL;
    sd_bus *bus = NULL;
    char *return_string;
    int r;

    /* Connect to the bus */
#ifdef USER_DBUS
    r = sd_bus_open_user(&bus);
    if (r < 0)
        dbus_error("Failed to connect to user bus:", r);
#else
    r = sd_bus_open_system(&bus);
    if (r < 0)
        dbus_error("Failed to connect to system bus:", r);
#endif

    /* Issue the method call and store the response message in m */
    r = sd_bus_call_method(bus,
                           BUS_NAME,
                           OBJECT_PATH,
                           INTERFACE_NAME,
                           "Command",
                           &error,
                           &m,
                           "s",
                           "THIS IS A TEST");
    if (r < 0)
            dbus_error("Failed to issue method call:", r);

    /* Parse the response message */
    r = sd_bus_message_read(m, "s", &return_string);
    if (r < 0)
            dbus_error("Failed to parse response message:", r);

    printf("%s\n", return_string);

    sd_bus_error_free(&error);
    sd_bus_message_unref(m);
    sd_bus_unref(bus);

    return 1;
}
