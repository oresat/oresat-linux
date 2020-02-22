#include "testprucam.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <stddef.h>
#include <errno.h>
#include <pthread.h>
#include <systemd/sd-bus.h>


#define DESTINATION     "org.OreSat.StarTracker"
#define BUS_NAME        "org.OreSat.StarTracker"
#define OBJECT_PATH     "/org/OreSat/StarTracker"


/* static data */
static const sd_bus_vtable method_vtable[];
static sd_bus_slot *slot = NULL;
static sd_bus *bus = NULL;
static bool endProgram = 0;
static char file_name[50];
static char file_path[200];
static uint8_t capture_num = 0;


/* static functions */
static int lastest_image(sd_bus_message *m, void *systemdata, sd_bus_error *ret_error);


/******************************************************************************/


void dbusAssertError(int r, char* err) {
    if (r < 0)
        fprintf(stderr, "%s %s\n", err, strerror(-r));
    return;
}


void dbusAssertFailure(int r, char* err) {
    if (r < 0) {
        fprintf(stderr, "%s %s\n", err, strerror(-r));
        exit(0);
    }
    return;
}


/******************************************************************************/


static int lastest_image(sd_bus_message *m, void *systemdata, sd_bus_error *ret_error) {
    int r;
    sprintf(file_name, "captures/capture_%03d.bmp", capture_num);
    get_image(file_name);
    ++capture_num;
    if(realpath(file_name, file_path) == NULL)
        dbusAssertError(r, "latest image file path error.");

    return sd_bus_reply_method_return(m, "s", file_path);
}


static const sd_bus_vtable method_vtable[] = {
        SD_BUS_VTABLE_START(0),
        SD_BUS_METHOD("LatestImage", NULL, "s", lastest_image, SD_BUS_VTABLE_UNPRIVILEGED),
        SD_BUS_VTABLE_END
};


int main(int argc, char *argv[]) {
    int r;

    if(getuid() != 0) {
        printf("This uses system dbus. Run as root.\n");
        return 1;
    }
    
    /* Connect to the bus */
    r = sd_bus_open_system(&bus);
    dbusAssertFailure(r, "Failed to connect to system bus.");

    /* Take a well-known service //name so that clients can find us */
    r = sd_bus_request_name(bus, BUS_NAME, SD_BUS_NAME_REPLACE_EXISTING);
    dbusAssertFailure(r, "Failed to acquire service name.");

    /* Install the object */
    r = sd_bus_add_object_vtable(bus,
                                 &slot,
                                 OBJECT_PATH,
                                 DESTINATION,
                                 method_vtable,
                                 NULL);
    dbusAssertFailure(r, "Failed to issue method call:");
    
    while(endProgram == 0) {
        /* Process requests */
        r = sd_bus_process(bus, NULL);
        dbusAssertError(r, "Failed to process bus:");

        if (r > 0) /* we processed a request, try to process another one, right-away */
            continue;

        /* Wait for the next request to process */
        r = sd_bus_wait(bus, (uint64_t) -1);
        dbusAssertError(r, "Failed to wait on bus:");
    }

    r = sd_bus_release_name(bus, BUS_NAME);
    dbusAssertError(r, "Failed to release service name.");
    sd_bus_slot_unref(slot);
    sd_bus_unref(bus);

    return 0;
}


