/*
 * DBUS
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <systemd/sd-bus.h>


#include "CANopen.h"
#include "CO_master.h"
#include "dbus.h"
#include "dbus_helpers.h"


#define INTERFACE_NAME  "org.example.project.oresat"
#define BUS_NAME        INTERFACE_NAME
#define OBJECT_PATH     "/org/example/project/oresat"
#define SIGNAL_THREAD_STACK_SIZE STRING_BUFFER_SIZE*3

/* Variables */
static pthread_t            signal_thread_id;
static pthread_attr_t       signal_thread_attr;
static volatile int         endProgram = 0;

/* static functions */
static void*                signal_thread(void *);
static int                  read_file_signal_cb(sd_bus_message 
                                                *m, 
                                                void *user_data, 
                                                sd_bus_error 
                                                *ret_error);


static void dbus_error(char* err, int r) {
    endProgram = 0;
    fprintf(stderr, "%s %s\n", err, strerror(-r));
    exit(0);
}


int dbus_init(void) {

    if(CO == NULL || CO->SDOclient == NULL){
        CO_errExit("dbus_init - Wrong arguments");
    }

    /* Create thread */
    endProgram = 0;
    pthread_attr_setstacksize(&signal_thread_attr, SIGNAL_THREAD_STACK_SIZE);
    if(pthread_create(&signal_thread_id, &signal_thread_attr, signal_thread, NULL) != 0) {
        CO_errExit("dbus_init - thread creation failed");
    }

    return 0;
}


int dbus_clear(void) {

    // TODO code here

    endProgram = 1;

    /* Wait for thread to finish. */
    if(pthread_join(signal_thread_id, NULL) != 0) {
        return -1;
    }

    return 0;
}


static int read_file_signal_cb(sd_bus_message *m, void *user_data, sd_bus_error *ret_error) {
    int r;
    char *filepath = NULL;
    endProgram = 1;

    r = sd_bus_message_read(m, "s", &filepath);
    if(r < 0) {
        fprintf(stderr, "Failed to parse data signal %s\n", strerror(-r));
        return -1;
    }

    send_file(filepath,
          0x1201, // idx
          1,      // subidx for file name
          2);     // subidx for file data

    return 0;
}


static void* signal_thread(void *arg) {
    sd_bus_slot *slot = NULL;
    sd_bus *bus = NULL;
    int r;

    /* Connect to the user bus */
    r = sd_bus_open_system(&bus);
    if(r < 0)
        dbus_error("Failed to connect to system bus:", -r);

    r = sd_bus_match_signal(bus,
                            &slot,
                            NULL,
                            OBJECT_PATH,
                            INTERFACE_NAME,
                            "file_signal", 
                            read_file_signal_cb, 
                            NULL);
    if(r < 0)
        dbus_error("Add match error:", r);

    while(endProgram == 0) {
        r = sd_bus_process(bus, NULL);
        if(r < 0)
            dbus_error("Bus Process failed:", r);
        if(r > 0)
            continue;

        r = sd_bus_wait(bus, (uint64_t)-1);
        if(r < 0)
            dbus_error("Bus wait failed:", r);
    }

    sd_bus_slot_unref(slot);
    sd_bus_unref(bus);
    return NULL;
}
