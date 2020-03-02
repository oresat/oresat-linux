#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <errno.h>
#include <systemd/sd-bus.h>


#define DESTINATION     "org.OreSat.Example"
#define INTERFACE_NAME  "org.OreSat.Example"
#define OBJECT_PATH     "/org/OreSat/Example"


// callback for reading the data signal
static int hello_signal_cb(sd_bus_message *m, void *user_data, sd_bus_error *ret_error) {
    int r;
    char * message_string = NULL;

    r = sd_bus_message_read(m, "s", &message_string);
    if(r < 0)
        fprintf(stderr, "Failed to parse hello signal");

    printf("%s\n", message_string);

    return 0;
}


// callback for reading the data signal
static int data_signal_cb(sd_bus_message *m, void *user_data, sd_bus_error *ret_error) {
    int r;
    double message_double = 0.0;
    int32_t message_int = 0;

    r = sd_bus_message_read(m, "id", &message_int, &message_double);
    if(r < 0)
        fprintf(stderr, "Failed to parse data signal");

    printf("%d %f\n", message_int, message_double);

    return 0;
}


int main(int argc, char *argv[]) {
    sd_bus_slot *slot = NULL;
    sd_bus *bus = NULL;
    int r;

    // Connect to the bus
    r = sd_bus_open_system(&bus);
    if(r < 0) {
        fprintf(stderr, "Failed to connect to system bus.");
        exit(EXIT_FAILURE);
    }

    r = sd_bus_add_match(
            bus,
            &slot,
            "type='signal', path='"OBJECT_PATH"', interface='"INTERFACE_NAME"', member='HelloSignal'",
            hello_signal_cb,
            NULL);
    if(r < 0) {
        fprintf(stderr, "Add data signal match error.");
        exit(EXIT_FAILURE);
    }

    r = sd_bus_add_match(
            bus,
            &slot,
            "type='signal', path='"OBJECT_PATH"', interface='"INTERFACE_NAME"', member='DataSignal'",
            data_signal_cb,
            NULL);
    if(r < 0) {
        fprintf(stderr, "Add data signal match error.");
        exit(EXIT_FAILURE);
    }

    while(1) {
        r = sd_bus_process(bus, NULL);
        if(r < 0)
            fprintf(stderr, "Bus Process failed.");
        else if(r > 0)
            continue;

        if(sd_bus_wait(bus, (uint64_t)-1) < 0)
            fprintf(stderr, "Bus wait failed.");
    }

    sd_bus_slot_unref(slot);
    sd_bus_unref(bus);
    return 1;
}

