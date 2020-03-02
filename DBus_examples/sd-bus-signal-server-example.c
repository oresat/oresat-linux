#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <errno.h>
#include <systemd/sd-bus.h>


#define DESTINATION     "org.OreSat.Example"
#define INTERFACE_NAME  "org.OreSat.Example"
#define OBJECT_PATH     "/org/OreSat/Example"
#define WAIT_TIME       1 // seconds


static const sd_bus_vtable vtable[] = {
    SD_BUS_VTABLE_START(0),
    // key: SD_BUS_METHOD(dbus_signal_name, data_types, flag),
    SD_BUS_SIGNAL("HelloSignal", "s", 0),
    SD_BUS_SIGNAL("DataSignal", "id", 0),
    SD_BUS_VTABLE_END
};


int main(int argc, char *argv[]) {
    sd_bus_slot *slot = NULL;
    sd_bus *bus = NULL;
    char test_string[] = "Hello world!";
    int32_t test_int = 5;
    double test_double = 10.0;
    int data_time_out = 0;
    int r;

    // Connect to the bus
    r = sd_bus_open_system(&bus);
    if(r < 0) {
        fprintf(stderr, "Failed to connect to system bus.");
        exit(EXIT_FAILURE);
    }

    // Take a well-known service name so that clients can find us
    r = sd_bus_request_name(bus, DESTINATION, SD_BUS_NAME_ALLOW_REPLACEMENT);
    if(r < 0) {
        fprintf(stderr, "Failed to acquire service name. \nIs "INTERFACE_NAME".conf in /etc/dbus-1/system.d/ ?");
        exit(EXIT_FAILURE);
    }

    // Install the vtable
    r = sd_bus_add_object_vtable(
            bus,
            &slot,
            OBJECT_PATH,
            INTERFACE_NAME,
            vtable,
            NULL);
    if(r < 0) {
        fprintf(stderr, "Failed to add vtable.");
        exit(EXIT_FAILURE);
    }

    while(1) {
        // send hello signal
        r = sd_bus_emit_signal(
                bus,
                OBJECT_PATH,
                INTERFACE_NAME,
                "HelloSignal",
                "s",
                test_string);
        if(r < 0) {
            fprintf(stderr, "Hello signal message failed:");
            goto END;
        }

        printf("%s\n", test_string);

        if(data_time_out >= 3) {
            data_time_out = 0;

            // send data signal
            r = sd_bus_emit_signal(
                    bus,
                    OBJECT_PATH,
                    INTERFACE_NAME,
                    "DataSignal",
                    "id",
                    test_int,
                    test_double);
            if(r < 0) {
                fprintf(stderr, "Data signal message failed:");
                goto END;
            }

            printf("%d %f\n", test_int, test_double);
        }

        ++data_time_out;

END:
        sleep(WAIT_TIME);
    }

    r = sd_bus_release_name(bus, DESTINATION);
    fprintf(stderr, "Failed to release service name.");

    sd_bus_slot_unref(slot);
    sd_bus_unref(bus);
    return 1;
}

