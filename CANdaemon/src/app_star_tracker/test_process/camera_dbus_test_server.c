#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <stddef.h>
#include <errno.h>
#include <pthread.h>
#include <systemd/sd-bus.h>

#include "testprucam.h"

#define INTERFACE_NAME  "org.OreSat.OreSat"
#define BUS_NAME        INTERFACE_NAME
#define OBJECT_PATH     "/org/OreSat/OreSat"
#define WAIT_TIME       500000 // mircroseconds

static sd_bus *bus = NULL;
static bool endProgram = 0;
static int wait_time = WAIT_TIME;


static void dbus_assert(const int r, const char* err) {
    if (r < 0)
        fprintf(stderr, "%s %s\n", err, strerror(-r));
    return;
}


int main(int argc, char *argv[]) {
    int r;

    if(getuid() != 0) {
        printf("This uses system dbus. Run as root.\n");
        return 1;
    }
    
    /* Connect to the bus */
    r = sd_bus_open_system(&bus);
    dbus_assert(r, "Failed to connect to system bus.");

    /* Take a well-known service name so that clients can find us */
    r = sd_bus_request_name(bus, BUS_NAME, SD_BUS_NAME_REPLACE_EXISTING);
    dbus_assert(r, "Failed to acquire service name.");
    if( r < 0)
        exit(1);

    /* file name */
    char name[20];
    sprintf(name, "capture_%03d.bmp", 1);

    while(endProgram == 0) {
        get_image(name);

        /* send signal */
        r = sd_bus_emit_signal(bus, 
                               OBJECT_PATH, 
                               INTERFACE_NAME,
                               "NewImageSignal", 
                               "s", 
                               name);
        dbus_assert(r, "New image signal message failed.");

        usleep(wait_time);
    }

    r = sd_bus_release_name(bus, BUS_NAME);
    dbus_assert(r, "Failed to release service name.");
    sd_bus_unref(bus);

    return 0;
}

