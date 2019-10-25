#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <errno.h>
#include <systemd/sd-bus.h>


#define INTERFACE_NAME  "org.example.project.oresat"
#define BUS_NAME        INTERFACE_NAME
#define OBJECT_PATH     "/org/example/project/oresat"
#define WAIT_TIME       500000 // mircroseconds


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
    int r;

    /* Connect to the bus */
    r = sd_bus_open_system(&bus);
    dbusErrorExit(r, "Failed to connect to system bus.");

    while(endProgram == 0) {
        /* check for dbus property */
        r = sd_bus_get_property(bus,
                            BUS_NAME,
                            OBJECT_PATH,
                            INTERFACE_NAME,
                            "TestProp",
                            &err,
                            &mess,
                            "d");
        dbusError(r, "Get property failed.");

        /* decode dbus property */
        r = sd_bus_message_read(mess, "d", &test_double);
        dbusError(r, "Read message failed.");

        printf("%f\n", test_double);
    }

    sd_bus_error_free(&err);
    sd_bus_unref(bus);
    return 1;
}


/*****************************************************************************/
/* Server */


int server(void) {
    sd_bus_error err = SD_BUS_ERROR_NULL;
    int r;
    
    /* Connect to the bus */
    r = sd_bus_open_system(&bus);
    dbusErrorExit(r, "Failed to connect to system bus.");

    /* Take a well-known service name so that clients can find us */
    r = sd_bus_request_name(bus, BUS_NAME, SD_BUS_NAME_ALLOW_REPLACEMENT);
    dbusError(r, "Failed to acquire service name. \nIs "INTERFACE_NAME".conf in /etc/dbus-1/system.d/ ?");


    while(endProgram == 0) {
        // make dbus properties
        r = sd_bus_set_property(bus,
                            NULL,
                            OBJECT_PATH,
                            INTERFACE_NAME,
                            "Test",
                            &err,
                            "s",
                            "Hello");
        dbusError(r, "Set property failed.");

        r = sd_bus_set_property(bus,
                             BUS_NAME,
                             OBJECT_PATH,
                             INTERFACE_NAME,
                             "TestProp",
                             &err,
                             "d",
                             10.0);
        dbusErrorExit(r, "Set property error");

        usleep(WAIT_TIME);
    }

    r = sd_bus_release_name(bus, BUS_NAME);
    dbusError(r, "Failed to release service name.");

    sd_bus_error_free(&err);
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

