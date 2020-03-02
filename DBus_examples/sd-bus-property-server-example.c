#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stddef.h>
#include <stdbool.h>
#include <errno.h>
#include <systemd/sd-bus.h>
#include <pthread.h>


#define DESTINATION     "org.OreSat.Example"
#define INTERFACE_NAME  "org.OreSat.Example"
#define OBJECT_PATH     "/org/OreSat/Example"


typedef struct {
    double test1;
    uint32_t test2;
} Object;


static sd_bus *bus = NULL;
static Object test_obj = {12.3, 1};
static bool running = true;


static const sd_bus_vtable vtable[] = {
    SD_BUS_VTABLE_START(0),
    // key: SD_BUS_PROPERTY(dbus_property_name, data_types, getter_function, pointer_to_data, flag),
    SD_BUS_PROPERTY("Test1", "d", NULL, offsetof(Object, test1), SD_BUS_VTABLE_PROPERTY_EMITS_CHANGE),
    // key: SD_BUS_WRITABLE_PROPERTY(dbus_property_name, data_types, getter_function, setter_function, pointer_to_data, flag),
    SD_BUS_WRITABLE_PROPERTY("Test2", "u", NULL, NULL, offsetof(Object, test2), SD_BUS_VTABLE_PROPERTY_EMITS_CHANGE),
    SD_BUS_VTABLE_END
};


void *data_thread(void *arg);


int main(int argc, char *argv[]) {
    sd_bus_slot *slot = NULL;
    pthread_t thread_id;
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
        fprintf(stderr, "Failed to acquire service name.\n");
        exit(EXIT_FAILURE);
    }

    // Install the vtable
    r = sd_bus_add_object_vtable(
            bus,
            &slot,
            OBJECT_PATH,
            INTERFACE_NAME,
            vtable,
            &test_obj);
    if(r < 0) {
        fprintf(stderr, "Failed to add vtable.");
        exit(EXIT_FAILURE);
    }

    pthread_create(&thread_id, NULL, data_thread, NULL);

    while(running) {
        // Process requests
        r = sd_bus_process(bus, NULL);
        if(r < 0)
            fprintf(stderr, "Failed to process bus.");
        else if (r > 0) // we processed a request, try to process another one, right-away
            continue;

        // Wait for the next request to process
        r = sd_bus_wait(bus, (uint64_t) -1);
        if(r < 0)
            fprintf(stderr, "Failed to wait on bus.");
    }

    running = false;
     pthread_join(thread_id, NULL);

    r = sd_bus_release_name(bus, DESTINATION);
    if(r < 0)
        fprintf(stderr, "Failed to release service name.");

    sd_bus_slot_unref(slot);
    sd_bus_unref(bus);
    return EXIT_SUCCESS;
}


void *data_thread(void *arg){
    int r;
    while(running) {
        ++test_obj.test1;
        r = sd_bus_emit_properties_changed(
                bus,
                OBJECT_PATH,
                INTERFACE_NAME,
                "Test1",
                NULL);
        if(r < 0)
            fprintf(stderr, "Failed to release service name.");

        printf("%f\n",test_obj.test1);
        sleep(1);
    }

    return NULL;
}
