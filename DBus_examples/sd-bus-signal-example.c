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


/*****************************************************************************/
/* Since all dbus function return negative errno values on failure,
 * these functionare just useful easy assert functions.
 */


void dbus_assert_error(int r, char* err) {
    if (r < 0)
        fprintf(stderr, "%s %s\n", err, strerror(-r));
    return;
}


void dbus_assert_error_exit(int r, char* err) {
    if (r < 0) {
        fprintf(stderr, "%s %s\n", err, strerror(-r));
        exit(0);
    }
    return;
}


/*****************************************************************************/
/* client */


// callback for reading the data signal
static int hello_signal_cb(sd_bus_message *m, void *user_data, sd_bus_error *ret_error) {
    int r;
    char * message_string = NULL;

    r = sd_bus_message_read(m, "s", &message_string);
    dbus_assert_error(r, "Failed to parse hello signal");

    printf("%s\n", message_string);

    return 0;
}



// callback for reading the data signal
static int data_signal_cb(sd_bus_message *m, void *user_data, sd_bus_error *ret_error) {
    int r;
    double message_double = 0.0;
    int32_t message_int = 0;

    r = sd_bus_message_read(m, "id", &message_int, &message_double);
    dbus_assert_error(r, "Failed to parse data signal");

    printf("%d %f\n", message_int, message_double);

    return 0;
}


int client(void) {
    sd_bus_slot *slot = NULL;
    sd_bus *bus = NULL;
    int r;

    // Connect to the bus
    r = sd_bus_open_system(&bus);
    dbus_assert_error_exit(r, "Failed to connect to system bus.");

    r = sd_bus_add_match(
            bus,
            &slot,
            "type='signal', path='"OBJECT_PATH"', interface='"INTERFACE_NAME"', member='HelloSignal'",
            hello_signal_cb,
            NULL);
    dbus_assert_error_exit(r, "Add data signal match error.");

    r = sd_bus_add_match(
            bus,
            &slot,
            "type='signal', path='"OBJECT_PATH"', interface='"INTERFACE_NAME"', member='DataSignal'",
            data_signal_cb,
            NULL);
    dbus_assert_error_exit(r, "Add data signal match error.");

    while(1) {
        r = sd_bus_process(bus, NULL);
        dbus_assert_error(r, "Bus Process failed.");

        if(r > 0)
            continue;

        r = sd_bus_wait(bus, (uint64_t)-1);
        dbus_assert_error(r, "Bus wait failed.");
    }

    sd_bus_slot_unref(slot);
    sd_bus_unref(bus);

    return 1;
}


/*****************************************************************************/
/* Server */


static const sd_bus_vtable vtable[] = {
    SD_BUS_VTABLE_START(0),
    // key: SD_BUS_METHOD(dbus_signal_name, data_types, flag),
    SD_BUS_SIGNAL("HelloSignal", "s", 0),
    SD_BUS_SIGNAL("DataSignal", "id", 0),
    SD_BUS_VTABLE_END
};


int server(void) {
    sd_bus_slot *slot = NULL;
    sd_bus *bus = NULL;
    char test_string[] = "Hello world!";
    int32_t test_int = 5;
    double test_double = 10.0;
    int r;

    // Connect to the bus
    r = sd_bus_open_system(&bus);
    dbus_assert_error_exit(r, "Failed to connect to system bus.");

    // Take a well-known service name so that clients can find us
    r = sd_bus_request_name(bus, DESTINATION, SD_BUS_NAME_ALLOW_REPLACEMENT);
    dbus_assert_error(r, "Failed to acquire service name. \nIs "INTERFACE_NAME".conf in /etc/dbus-1/system.d/ ?");

    // Install the vtable
    r = sd_bus_add_object_vtable(
            bus,
            &slot,
            OBJECT_PATH,
            INTERFACE_NAME,
            vtable,
            NULL);
    dbus_assert_error(r, "Failed to add vtable.");

    while(1) {
        //send messages
        r = sd_bus_emit_signal(
                bus,
                OBJECT_PATH,
                INTERFACE_NAME,
                "HelloSignal",
                "s",
                test_string);
        dbus_assert_error(r, "Hello signal message failed:");

        printf("%s\n", test_string);

        //send messages
        r = sd_bus_emit_signal(
                bus,
                OBJECT_PATH,
                INTERFACE_NAME,
                "DataSignal",
                "id",
                test_int,
                test_double);
        dbus_assert_error(r, "Data signal message failed:");

        printf("%d %f\n", test_int, test_double);

        sleep(WAIT_TIME);
    }

    r = sd_bus_release_name(bus, DESTINATION);
    dbus_assert_error(r, "Failed to release service name.");

    sd_bus_slot_unref(slot);
    sd_bus_unref(bus);
    return 1;
}


/*****************************************************************************/
/* main */


static void print_input_error(void) {
    printf("Input error \nsudo ./sd-dbus-signal-example <Mode> \nWhere <Mode> is server or client.\n");
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

