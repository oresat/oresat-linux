#include <stdio.h>
#include <stdlib.h>
#include <systemd/sd-bus.h>

#define DESTINATION     "org.OreSat.Example"
#define INTERFACE_NAME  "org.OreSat.Example"
#define OBJECT_PATH     "/org/OreSat/Example"

static sd_bus *bus = NULL;

static int read_test2_cb(sd_bus_message *m, void *user_data, sd_bus_error *ret_error) {
    double test_double = 0;
    int r;

    r = sd_bus_get_property(
            bus,
            DESTINATION,
            OBJECT_PATH,
            INTERFACE_NAME,
            "Test1",
            ret_error,
            &m,
            "d");
    if (r < 0) {
        fprintf(stderr, "Get property failed.");
        return 0;
    }

    /* decode dbus property */
    r = sd_bus_message_read(m, "d", &test_double);
    if (r<0)
        fprintf(stderr, "Read message failed.");
    else
        printf("%f\n", test_double);

    return 0;
}


int main(int argc, char *argv[]) {
    sd_bus_error err = SD_BUS_ERROR_NULL;
    void* userdata = NULL;

    /* Connect to the bus */
    if(sd_bus_open_system(&bus) < 0) {
        fprintf(stderr, "Failed to connect to systemd bus.\n");
        exit(0);
    }

    int r = sd_bus_match_signal(
            bus,
            NULL,
            NULL,
            OBJECT_PATH,
            "org.freedesktop.DBus.Properties",
            "PropertiesChanged",
            read_test2_cb,
            userdata
        );
    if(r < 0) {
        fprintf(stderr, "Failed to add new signal match.\n");
        exit(0);
    }

    while(1) {
        /* Process requests */
        r = sd_bus_process(bus, NULL);
        if ( r < 0)
            fprintf(stderr, "Failed to processA bus.\n");
        else if (r > 0) /* we processed a request, try to process another one, right-away */
            continue;

        /* Wait for the next request to process */
        if (sd_bus_wait(bus, UINT64_MAX) < 0) {
            fprintf(stderr, "Bus wait failed.\n");
        }

    }

    sd_bus_error_free(&err);
    sd_bus_unref(bus);

    return 0;
}

