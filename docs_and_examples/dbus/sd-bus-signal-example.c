#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <errno.h>
#include <systemd/sd-bus.h>
#include <pthread.h>


#define INTERFACE_NAME  "org.example.project.oresat"
#define BUS_NAME        INTERFACE_NAME
#define OBJECT_PATH     "/org/example/project/oresat"
#define WAIT_TIME       1000000 // mircroseconds


/* static data */
static sd_bus_slot *slot = NULL;
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


// callback for reading the data signal
static int hello_signal_cb(sd_bus_message *m, void *user_data, sd_bus_error *ret_error) {
    int r;
    char * message_string = NULL;

    r = sd_bus_message_read(m, "s", &message_string);
    dbusError(r, "Failed to parse hello signal");

    printf("%s\n", message_string);

    return 0;
}



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
                         "type='signal', path='"OBJECT_PATH"', interface='"INTERFACE_NAME"', member='HelloSignal'", 
                         hello_signal_cb, 
                         NULL);
    dbusErrorExit(r, "Add data signal match error.");

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


static const sd_bus_vtable vtable[] = {
    SD_BUS_VTABLE_START(0),
    // key: SD_BUS_METHOD(dbus_signal_name, data_types, flag),
    SD_BUS_SIGNAL("HelloSignal", "s", 0),
    SD_BUS_SIGNAL("DataSignal", "id", 0),
    SD_BUS_VTABLE_END
};

void* send_signals_thread(void *arg) {
    char test_string[] = "Hello world!";
    int32_t test_int = 5;
    double test_double = 10.0;
    int r;

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
        dbusError(r, "Data signal message failed:");

        printf("%d %f\n", test_int, test_double);

        usleep(WAIT_TIME);
    }
}

int server(void) {
    pthread_t send_signals_thread_id;
    int r;
    
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
                                 NULL);
    dbusError(r, "Failed to add vtable.");

    pthread_create(&send_signals_thread_id, NULL, send_signals_thread, NULL);

    while(endProgram == 0) {
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

