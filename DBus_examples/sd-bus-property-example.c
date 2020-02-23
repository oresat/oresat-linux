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


/*****************************************************************************/
/* Since all dbus function return negative errno values on failure,
 * these functionare just useful easy assert functions.
 */


void dbus_assert(int r, char* err) {
    if (r < 0)
        fprintf(stderr, "%s %s\n", err, strerror(-r));
    return;
}


void dbus_assert_exit(int r, char* err) {
    if (r < 0) {
        fprintf(stderr, "%s %s\n", err, strerror(-r));
        exit(0);
    }
    return;
}


/*****************************************************************************/
/* client */


int client(void) {
    sd_bus *bus = NULL;
    sd_bus_error err = SD_BUS_ERROR_NULL;
    sd_bus_message *mess = NULL;
    double test_double = 0.0;
    uint32_t test_int = 0;
    bool endProgram = 0;
    int r;

    // Connect to the bus
    r = sd_bus_open_system(&bus);
    dbus_assert_exit(r, "Failed to connect to system bus.");

    while(endProgram == 0) {
        // check for dbus property
        r = sd_bus_get_property(bus,DESTINATION, OBJECT_PATH, INTERFACE_NAME, "Test1", &err, &mess, "d");
        dbus_assert(r, "Get property failed.");

        if (r >= 0) {
            /* decode dbus property */
            r = sd_bus_message_read(mess, "d", &test_double);
            dbus_assert(r, "Read message failed.");

            printf("%f\n", test_double);
        }

        /* free message */
        sd_bus_error_free(&err);
        err = SD_BUS_ERROR_NULL;
        sd_bus_message_unref(mess);
        mess = NULL;

        r = sd_bus_get_property(bus, DESTINATION, OBJECT_PATH, INTERFACE_NAME, "Test2",  &err, &mess, "u");
        dbus_assert(r, "Get property failed.");

        if (r >= 0) {
            // decode dbus property/
            r = sd_bus_message_read(mess, "u", &test_int);
            dbus_assert(r, "Read message failed.");

            printf("%d\n", test_int);
        }

        // free message
        sd_bus_error_free(&err);
        err = SD_BUS_ERROR_NULL;
        sd_bus_message_unref(mess);
        mess = NULL;

        ++test_int;

        if (r >= 0) {
            r = sd_bus_set_property(bus, DESTINATION, OBJECT_PATH, INTERFACE_NAME, "Test2",  &err, "u", test_int);
            dbus_assert(r, "Set property failed.");
            printf("%d\n", test_int);
        }

        printf("\n");

        sleep(WAIT_TIME);
    }

    sd_bus_error_free(&err);
    sd_bus_unref(bus);
    return 1;
}


/*****************************************************************************/
/* Server */


typedef struct {
    double test1;
    uint32_t test2;
} Object;


static const sd_bus_vtable vtable[] = {
    SD_BUS_VTABLE_START(0),
    // key: SD_BUS_PROPERTY(dbus_property_name, data_types, getter_function, pointer_to_data, flag),
    SD_BUS_PROPERTY("Test1", "d", NULL, offsetof(Object, test1), SD_BUS_VTABLE_PROPERTY_EMITS_CHANGE),
    // key: SD_BUS_WRITABLE_PROPERTY(dbus_property_name, data_types, getter_function, setter_function, pointer_to_data, flag),
    SD_BUS_WRITABLE_PROPERTY("Test2", "u", NULL, NULL, offsetof(Object, test2), SD_BUS_VTABLE_PROPERTY_EMITS_CHANGE),
    SD_BUS_VTABLE_END
};


int server(void) {
    sd_bus *bus = NULL;
    sd_bus_slot *slot = NULL;
    int r;

    Object testObj;
    testObj.test1 = 12.3;
    testObj.test2 = 1;

    // Connect to the bus
    r = sd_bus_open_system(&bus);
    dbus_assert_exit(r, "Failed to connect to system bus.");

    // Take a well-known service name so that clients can find us
    r = sd_bus_request_name(bus, DESTINATION, SD_BUS_NAME_ALLOW_REPLACEMENT);
    dbus_assert(r, "Failed to acquire service name. \nIs "INTERFACE_NAME".conf in /etc/dbus-1/system.d/ ?");

    // Install the vtable
    r = sd_bus_add_object_vtable(bus,
                                 &slot,
                                 OBJECT_PATH,
                                 INTERFACE_NAME,
                                 vtable,
                                 &testObj);
    dbus_assert(r, "Failed to add vtable.");

    for(;;) {
        // Process requests
        r = sd_bus_process(bus, NULL);
        dbus_assert(r, "Failed to process bus.");

        if (r > 0) // we processed a request, try to process another one, right-away
            continue;

        // Wait for the next request to process
        r = sd_bus_wait(bus, (uint64_t) -1);
        dbus_assert(r, "Failed to wait on bus.");
    }

    r = sd_bus_release_name(bus, DESTINATION);
    dbus_assert(r, "Failed to release service name.");

    sd_bus_slot_unref(slot);
    sd_bus_unref(bus);
    return 1;
}


/*****************************************************************************/
/* main */


static void print_input_error(void) {
    printf("Input error \nsudo ./sd-dbus-property-example <Mode> \nWhere <Mode> is server or client.\n");
}


int main(int argc, char *argv[]) {
    int ret = 0;

    if(argc != 2)
        print_input_error();

    if(strncmp(argv[1], "server", strlen(argv[1])) == 0)
        ret = server();
    else if(strncmp(argv[1], "client", strlen(argv[1])) == 0)
        ret = client();
    else
        print_input_error();

    return ret;
}

