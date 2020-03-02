#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stddef.h>
#include <stdbool.h>
#include <errno.h>
#include <systemd/sd-bus.h>


#define DESTINATION     "org.OreSat.Example"
#define INTERFACE_NAME  "org.OreSat.Example"
#define OBJECT_PATH     "/org/OreSat/Example"
#define WAIT_TIME       1 // seconds


int main(int argc, char *argv[]) {
    sd_bus *bus = NULL;
    sd_bus_error err = SD_BUS_ERROR_NULL;
    sd_bus_message *mess = NULL;
    double test_double = 0.0;
    uint32_t test_int = 0;
    bool endProgram = 0;
    int r;

    // Connect to the bus
    r = sd_bus_open_system(&bus);
    if(r < 0) {
        fprintf(stderr, "Failed to connect to system bus.");
        exit(EXIT_FAILURE);
    }

    while(endProgram == 0) {
        // check for dbus property
        r = sd_bus_get_property(bus,DESTINATION, OBJECT_PATH, INTERFACE_NAME, "Test1", &err, &mess, "d");
        if(r < 0) {
            fprintf(stderr, "Get Test1 property failed.");
            goto END;
        }

        /* decode dbus property */
        r = sd_bus_message_read(mess, "d", &test_double);
        if(r < 0) {
            fprintf(stderr, "Read Test1 message failed.");
            goto END;
        }

        printf("%f\n", test_double);

        /* free message */
        sd_bus_error_free(&err);
        err = SD_BUS_ERROR_NULL;
        sd_bus_message_unref(mess);
        mess = NULL;

        r = sd_bus_get_property(bus, DESTINATION, OBJECT_PATH, INTERFACE_NAME, "Test2",  &err, &mess, "u");
        if(r < 0) {
            fprintf(stderr, "Get Test2 property failed.");
            goto END;
        }

        // decode dbus property/
        r = sd_bus_message_read(mess, "u", &test_int);
        if(r < 0) {
            fprintf(stderr, "Read Test2 message failed.");
            goto END;
        }

        printf("%d\n", test_int);

        // free message
        sd_bus_error_free(&err);
        err = SD_BUS_ERROR_NULL;
        sd_bus_message_unref(mess);
        mess = NULL;

        r = sd_bus_set_property(bus, DESTINATION, OBJECT_PATH, INTERFACE_NAME, "Test2",  &err, "u", ++test_int);
        if(r < 0) {
            fprintf(stderr, "Set property failed.");
            goto END;
        }

        printf("\n");

END:
        // free message
        sd_bus_error_free(&err);
        err = SD_BUS_ERROR_NULL;
        sd_bus_message_unref(mess);
        mess = NULL;

        sleep(WAIT_TIME);
    }

    sd_bus_error_free(&err);
    sd_bus_unref(bus);
    return 1;
}

