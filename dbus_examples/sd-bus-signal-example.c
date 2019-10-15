#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <errno.h>
#include <systemd/sd-bus.h>


#define INTERFACE_NAME  "org.example.project.oresat"
#define BUS_NAME        INTERFACE_NAME
#define OBJECT_PATH     "/org/example/project/oresat"
#define WAIT_TIME       1000000 // mircroseconds


/* static data */
static sd_bus_slot *slot = NULL;
static sd_bus *bus = NULL;


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


// callback for reading the data signal
static int data_signal_cb(sd_bus_message *m, void *user_data, sd_bus_error *ret_error) {
    int r;
    double message_double = 0.0;
    int32_t message_int = 0;

    r = sd_bus_message_read(m, "id", &message_int, &message_double);
    dbusError(r, "Failed to parse data signal");

    printf("%d %f\n", message_int, message_double);

    return 0;
}


int client(void) {
    int r;

    /* Connect to the bus */
    r = sd_bus_open_system(&bus);
    dbusErrorExit(r, "Failed to connect to system bus.");

    r = sd_bus_add_match(bus,
                         &slot,
                         "type='signal', path='"OBJECT_PATH"', interface='"INTERFACE_NAME"', member='DataSignal'", 
                         data_signal_cb, 
                         NULL);
    dbusErrorExit(r, "Add data signal match error.");

    while(1) {
        r = sd_bus_process(bus, NULL);
        dbusError(r, "Bus Process failed.");

        if(r > 0)
            continue;

        r = sd_bus_wait(bus, (uint64_t)-1);
        dbusError(r, "Bus wait failed.");
    }

    sd_bus_slot_unref(slot);
    sd_bus_unref(bus);

    return 1;
}


/*****************************************************************************/
/* Server */


int server(void) {
    char test_string[] = "Hello world!";
    int32_t test_int = 5;
    double test_double = 10.0;
    int r;
    
    /* Connect to the bus */
    r = sd_bus_open_system(&bus);
    dbusErrorExit(r, "Failed to connect to system bus.");

    /* Take a well-known service name so that clients can find us */
    r = sd_bus_request_name(bus, BUS_NAME, SD_BUS_NAME_ALLOW_REPLACEMENT);
    dbusErrorExit(r, "Failed to acquire service name. \nIs org.example.project.oresat.conf in /etc/dbus-1/system.d/ or /usr/share/dbus-1/system.d/ ?");

    while(1) {
        //send messages
        r = sd_bus_emit_signal(bus, 
                               OBJECT_PATH, 
                               INTERFACE_NAME,
                               "HelloSignal", 
                               "s", 
                               test_string);
        dbusError(r, "Hello signal message failed:");

        printf("%s\n", test_string);

        r = sd_bus_emit_signal(bus, 
                               OBJECT_PATH, 
                               INTERFACE_NAME,
                               "DataSignal", 
                               "id", 
                                test_int,
                                test_double);
        dbusError(r, " Data signal message failed:");

        printf("%d %f\n", test_int, test_double);

        usleep(WAIT_TIME);
    }

    sd_bus_slot_unref(slot);
    sd_bus_unref(bus);
    return 1;
}


/*****************************************************************************/
/* main */


static void print_input_error(void) {
    printf("Input error \nsudo ./sd-dbus-method-example <Mode> \nWhere <Mode> is server or client.\n");
}


int main(int argc, char *argv[]) {
    int ret = 0;

    if(argc != 2)
        print_input_error();

    if(strncmp(argv[1], "server", strlen(argv[1]) == 0))
        ret = server();
    else if(strncmp(argv[1], "client", strlen(argv[1]) == 0))
        ret = client();
    else
        print_input_error();

    return ret;
}

