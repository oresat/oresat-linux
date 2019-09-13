/*
 * DBUS
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <systemd/sd-bus.h>
#include <pthread.h>


#include "CANopen.h"
#include "CO_master.h"
#include "dbus.h"
#include "dbus_helpers.h"


#define INTERFACE_NAME  "org.example.project.oresat"
#define BUS_NAME        INTERFACE_NAME
#define OBJECT_PATH     "/org/example/project/oresat"
#define SIGNAL_THREAD_STACK_SIZE STRING_BUFFER_SIZE*3

/* Variables */
static pthread_t        signal_thread_id;
static pthread_attr_t   signal_thread_attr;
static volatile int     endProgram = 0;

/* static functions */
static void* signal_thread(void *);


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
    endProgram = 1;

    /* Wait for thread to finish. */
    if(pthread_join(signal_thread_id, NULL) != 0) {
        return -1;
    }

    return 0;
}


static int file_transfer_signal_cb(sd_bus_message *m, void *user_data, sd_bus_error *ret_error) {
    int r;
    char *filepath = NULL;

    r = sd_bus_message_read(m, "s", &filepath);
    dbusError(r, "Failed to parse file transfer signal.");
    if (r > 0)
        return -1;

    OD_add_file(0x1201, 1, 2, filepath);

    free(filepath);
    filepath = NULL;
    return 0;
}


// callback for reading the data signal
static int data_signal_cb(sd_bus_message *m, void *user_data, sd_bus_error *ret_error) {
    int r;
    int16_t rotationY = 0; // roatation about the y axis
    int16_t rotationZ = 0; // rotation about the z axis
    int16_t orientation = 0; // rotation about the camera axis

    r = sd_bus_message_read(m, "nnn", &rotationY, &rotationZ, &orientation);
    dbusError(r, "Failed to parse data signal.");

    if (r > 0)
        return -1;

    fprintf(stderr, "%d %d %d\n", rotationY, rotationZ, orienation);

    OD_update(0x3000, 1, rotationY);
    OD_update(0x3000, 2, rotationZ);
    OD_update(0x3000, 3, orientation);

    return 0;
}


static void* signal_thread(void *arg) {
    sd_bus *bus = NULL;
    int r;

    /* Connect to the user bus */
    r = sd_bus_open_system(&bus);
    dbusErrorExit(r, "Failed to connect to system bus.");

    /* add signal matches */
    r = sd_bus_match_signal(bus,
                            NULL,
                            NULL,
                            OBJECT_PATH,
                            INTERFACE_NAME,
                            "DataSignal", 
                            data_signal_cb, 
                            NULL);
    dbusErrorExit(r, "Add match error for data signal.");

    r = sd_bus_match_signal(bus,
                            NULL,
                            NULL,
                            OBJECT_PATH,
                            INTERFACE_NAME,
                            "FileTransferSignal", 
                            file_transfer_signal_cb, 
                            NULL);
    dbusErrorExit(r, "Add match error for file transfer signal.");

    while(endProgram == 0) {
        r = sd_bus_process(bus, NULL);
        dbusError(r, "Bus process failed.");
        if(r > 0)
            continue;

        r = sd_bus_wait(bus, (uint64_t)-1);
        dbusError(r, "Bus wait failed.");
    }

    sd_bus_unref(bus);
    return NULL;
}
