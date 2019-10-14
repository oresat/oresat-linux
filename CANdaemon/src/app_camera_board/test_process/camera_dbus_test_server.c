#include "testprucam.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <stddef.h>
#include <errno.h>
#include <pthread.h>
#include <systemd/sd-bus.h>


#define INTERFACE_NAME  "org.OreSat.CameraBoard"
#define BUS_NAME        INTERFACE_NAME
#define OBJECT_PATH     "/org/OreSat/CameraBoard"
#define WAIT_TIME       500000 // mircroseconds


/* static data */
static const sd_bus_vtable method_vtable[];
static sd_bus_slot *slot = NULL;
static sd_bus *bus = NULL;
static bool endProgram = 0;
static int wait_time = WAIT_TIME;
static pthread_t method_thread_id;
static char name[50];


/* static functions */
static void* method_thread(void *arg);
int method_thread_init(void);
int method_thread_clear(void);
static int lastest_image(sd_bus_message *m, void *systemdata, sd_bus_error *ret_error);


static void dbus_assert(const int r, const char* err) {
    if (r < 0)
        fprintf(stderr, "%s %s\n", err, strerror(-r));
    return;
}


int main(int argc, char *argv[]) {
    int r;
    uint8_t capture_num = 0;

    if(getuid() != 0) {
        printf("This uses system dbus. Run as root.\n");
        return 1;
    }
    
    /* Connect to the bus */
    r = sd_bus_open_system(&bus);
    dbus_assert(r, "Failed to connect to system bus.");

    /* Take a well-known service //name so that clients can find us */
    r = sd_bus_request_name(bus, BUS_NAME, SD_BUS_NAME_REPLACE_EXISTING);
    dbus_assert(r, "Failed to acquire service name.");
    if( r < 0)
        exit(1);

    method_thread_init();

    while(endProgram == 0) {
        sprintf(name, "capture_%03d.bmp", capture_num);
        get_image(name);

        /* send signal */
        r = sd_bus_emit_signal(bus, 
                               OBJECT_PATH, 
                               INTERFACE_NAME,
                               "NewImageSignal", 
                               "s", 
                               name);
        dbus_assert(r, "New image signal message failed.");

        ++capture_num;
        usleep(wait_time);
    }

    method_thread_clear();

    r = sd_bus_release_name(bus, BUS_NAME);
    dbus_assert(r, "Failed to release service name.");
    sd_bus_unref(bus);

    return 0;
}


// ***************************************************************************
// Methods and Method table


int method_thread_init(void) {
    int r;

    /* Install the object */
    r = sd_bus_add_object_vtable(bus,
                                 &slot,
                                 OBJECT_PATH,
                                 INTERFACE_NAME,
                                 method_vtable,
                                 NULL);
    dbus_assert(r, "Failed to issue method call:");
    
    if(pthread_create(&method_thread_id, NULL, method_thread, NULL) != 0) {
        fprintf(stderr, "method_thread_init - thread creation failed.\n");
        return -1;
    }

    return 1;
}


int method_thread_clear(void) {
    /* Wait for thread to finish. */
    if(pthread_join(method_thread_id, NULL) != 0) {
        fprintf(stderr, "method_thread_clear - thread join failed.\n");
        return -1;
    }

    sd_bus_slot_unref(slot);

    return 1;
}


static void* method_thread(void *arg) {
    int r;

    while(endProgram == 0) {
        /* Process requests */
        r = sd_bus_process(bus, NULL);
        dbus_assert(r, "Failed to process bus:");
        if (r > 0) /* we processed a request, try to process another one, right-away */
            continue;

        /* Wait for the next request to process */
        r = sd_bus_wait(bus, (uint64_t) -1);
        dbus_assert(r, "Failed to wait on bus:");
    }

    return NULL;
}


static int lastest_image(sd_bus_message *m, void *systemdata, sd_bus_error *ret_error) {
    //int r;
    //char* command;

    //r = sd_bus_message_read(m, "s", &command);
    //dbus_assert(r, "Failed to parse parameters:");

    return sd_bus_reply_method_return(m, "s", name);
}


static const sd_bus_vtable method_vtable[] = {
        SD_BUS_VTABLE_START(0),
        SD_BUS_METHOD("LastestImage", NULL, "x", lastest_image, SD_BUS_VTABLE_UNPRIVILEGED),
        SD_BUS_VTABLE_END
};


