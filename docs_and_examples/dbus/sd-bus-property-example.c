#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stddef.h>
#include <stdbool.h>
#include <errno.h>
#include <systemd/sd-bus.h>


#define INTERFACE_NAME  "org.example.project.oresat"
#define BUS_NAME        INTERFACE_NAME
#define OBJECT_PATH     "/org/example/project/oresat"
#define WAIT_TIME       1 // seconds


/* static data */
static sd_bus *bus = NULL;
static bool endProgram = 0;


/*****************************************************************************/


void dbusError(int r, char* err) {
    if (r < 0)
        fprintf(stderr, "%s %s\n", err, strerror(-r));
    return;
}


void dbusErrorExit(int r, char* err) {
    if (r < 0) {
        fprintf(stderr, "%s %s\n", err, strerror(-r));
        exit(0);
    }
    return;
}


/*****************************************************************************/
/* client */


int client(void) {
    sd_bus_error err;
    sd_bus_message *mess = NULL;
    double test_double = 0.0;
    uint32_t test_int = 0;
    int r;

    /* Connect to the bus */
    r = sd_bus_open_system(&bus);
    dbusErrorExit(r, "Failed to connect to system bus.");

    while(endProgram == 0) {
        /* check for dbus property */
        //  int sd_bus_get_property(sd_bus *bus, const char *destination, const char *path, const char *interface, const char *member, sd_bus_error *ret_error, sd_bus_message **reply, const char *type);
        r = sd_bus_get_property(            bus,                BUS_NAME,      OBJECT_PATH,        INTERFACE_NAME,            "Test1",                    &err,                  &mess,              "d");
        dbusError(r, "Get property failed.");

        /* decode dbus property */
        r = sd_bus_message_read(mess, "d", &test_double);
        dbusError(r, "Read message failed.");

        printf("%f\n", test_double);

        /* free message */
        sd_bus_message_unref(mess);
        mess = NULL;

        r = sd_bus_get_property(bus, BUS_NAME, OBJECT_PATH, INTERFACE_NAME, "Test2",  &err, &mess, "u");
        dbusError(r, "Get property failed.");

        /* decode dbus property */
        r = sd_bus_message_read(mess, "u", &test_int);
        dbusError(r, "Read message failed.");

        printf("%d\n", test_int);

        /* free message */
        sd_bus_message_unref(mess);
        mess = NULL;

        r = sd_bus_set_property(bus, BUS_NAME, OBJECT_PATH, INTERFACE_NAME, "Test2",  &err, "u", 10);
        dbusError(r, "Set property failed.");

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
    SD_BUS_PROPERTY("Test1", "d", NULL, offsetof(Object, test1), 0),
    // key: SD_BUS_WRITABLE_PROPERTY(dbus_property_name, data_types, getter_function, setter_function, pointer_to_data, flag),
    SD_BUS_WRITABLE_PROPERTY("Test2", "u", NULL, NULL, offsetof(Object, test2), 0),
    SD_BUS_VTABLE_END
};


int server(void) {
    sd_bus *bus = NULL;
    sd_bus_slot *slot = NULL;
    int r;
    
    Object testObj;
    testObj.test1 = 12.3;
    testObj.test2 = 1;

    /* Connect to the bus */
    r = sd_bus_open_system(&bus);
    dbusErrorExit(r, "Failed to connect to system bus.");

    /* Take a well-known service name so that clients can find us */
    r = sd_bus_request_name(bus, BUS_NAME, SD_BUS_NAME_ALLOW_REPLACEMENT);
    dbusError(r, "Failed to acquire service name. \nIs "INTERFACE_NAME".conf in /etc/dbus-1/system.d/ ?");

    /* Install the vtable */
    r = sd_bus_add_object_vtable(bus,
                                 &slot,
                                 OBJECT_PATH,
                                 INTERFACE_NAME,
                                 vtable,
                                 &testObj);
    dbusError(r, "Failed to add vtable.");

    for(;;) {
        /* Process requests */
        r = sd_bus_process(bus, NULL);
        dbusError(r, "Failed to process bus.");

        if (r > 0) /* we processed a request, try to process another one, right-away */
            continue;

        /* Wait for the next request to process */
        r = sd_bus_wait(bus, (uint64_t) -1);
        dbusError(r, "Failed to wait on bus.");
    }

    r = sd_bus_release_name(bus, BUS_NAME);
    dbusError(r, "Failed to release service name.");

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

