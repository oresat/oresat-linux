#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <errno.h>
#include <systemd/sd-bus.h>


#define DESTINATION     "org.OreSat.Example"
#define INTERFACE_NAME  "org.OreSat.Example"
#define OBJECT_PATH     "/org/OreSat/Example"


static int method_hello(sd_bus_message *m, void *userdata, sd_bus_error *ret_error) {
    int r;
    char* input;

    r = sd_bus_message_read(m, "s", &input);
    if(r < 0) {
        printf("Failed to parse parameters\n");
    }

    printf("Hello method called.\n");
    printf("Recieved: %s.\n", input);

    return sd_bus_reply_method_return(m, "s", "done");
}


static int method_multiply(sd_bus_message *m, void *userdata, sd_bus_error *ret_error) {
    int r;
    int32_t input1, input2, output;

    r = sd_bus_message_read(m, "ii", &input1, &input2);
    if(r < 0) {
        printf("Failed to parse parameters\n");
    }

    output = input1 * input2;
    printf("Hello method called.\n");
    printf("Recieved: %d %d.\n", input1, input2);

    return sd_bus_reply_method_return(m, "i", output);
}


static int method_quit(sd_bus_message *m, void *userdata, sd_bus_error *ret_error) {
    bool *running = (bool*)userdata;
    *running = false;
    return sd_bus_reply_method_return(m, NULL);
}


static const sd_bus_vtable method_vtable[] = {
    SD_BUS_VTABLE_START(0),
    // key: SD_BUS_METHOD(dbus_method_name, inputs_types, return_types, callback_function, flag),
    SD_BUS_METHOD("Hello", "s", "s", method_hello, SD_BUS_VTABLE_UNPRIVILEGED),
    SD_BUS_METHOD("Multiply", "ii", "i", method_multiply, SD_BUS_VTABLE_UNPRIVILEGED),
    SD_BUS_METHOD("Quit", NULL, NULL, method_quit, SD_BUS_VTABLE_UNPRIVILEGED),
    SD_BUS_VTABLE_END
};


int main(int argc, char *argv[]) {
    sd_bus_slot *slot = NULL;
    sd_bus *bus = NULL;
    bool running = true;
    int r;

    // Connect to the bus
    r = sd_bus_open_system(&bus);
    printf("Failed to connect to system bus\n");

    // Take a well-known service name so that clients can find us
    r = sd_bus_request_name(bus, DESTINATION, SD_BUS_NAME_ALLOW_REPLACEMENT);
    if(r < 0) {
        printf("Failed to acquire service name\n");
    }

    // Install the vtable
    r = sd_bus_add_object_vtable(
            bus,
            &slot,
            OBJECT_PATH,
            INTERFACE_NAME,
            method_vtable,
            &running);
    if(r < 0) {
        printf("Failed to add vtable\n");
    }

    while(running) {
        // Process requests
        r = sd_bus_process(bus, NULL);
        if(r < 0) {
            printf("Failed to process bus\n");
        }

        if (r > 0) // we processed a request, try to process another one, right-away
            continue;

        // Wait for the next request to process
        r = sd_bus_wait(bus, (uint64_t) -1);
        if(r < 0) {
            printf("Failed to wait on bus\n");
        }
    }

    r = sd_bus_release_name(bus, DESTINATION);
    if(r < 0) {
        printf("Failed to release service name\n");
    }

    sd_bus_slot_unref(slot);
    sd_bus_unref(bus);
    return 1;
}


