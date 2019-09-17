/*
 * Main Process interface.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <systemd/sd-bus.h>
#include <pthread.h>

#include "Updater_interface.h"
#include "dbus_helpers.h"

#define INTERFACE_NAME  "org.OreSat.LinuxUpdater"
#define BUS_NAME        INTERFACE_NAME
#define OBJECT_PATH     "/org/OreSat.LinuxUpdater"
#define SIGNAL_THREAD_STACK_SIZE STRING_BUFFER_SIZE*3

/* Static Variables */
static pthread_t        signal_thread_id;
static pthread_attr_t   signal_thread_attr;
static volatile int     endProgram = 0;
static sd_bus           *bus = NULL;

/* Static Functions */
static void* signal_thread(void *arg);
static int status_signal_cb(sd_bus_message *, void *, sd_bus_error *);
static int file_transfer_signal_cb(sd_bus_message *, void *, sd_bus_error *);
static int data_signal_cb(sd_bus_message *, void *, sd_bus_error *);
static void Updater_updateState(void);


int Updater_interface_init(void) {
    int r;
    endProgram = 0;

    /* Connect to the system bus */
    r = sd_bus_open_system(&bus);
    dbusErrorExit(r, "Failed to connect to system bus.");

    pthread_attr_setstacksize(&signal_thread_attr, SIGNAL_THREAD_STACK_SIZE);
    r = pthread_create(&signal_thread_id, &signal_thread_attr, Updater_signalThread, NULL);
    dbusError(-r, "dbus_init - signal thread creation failed");
    
    return r;
}


int Updater_interface_clear(void) {
    endProgram = 1;

    /* Wait for threads to finish. */
    if(pthread_join(method_thread_id, NULL) != 0) {
        return -1;
    }

    sd_bus_unref(bus);

    return 0;
}


/****************************************************************************/
/* singals / properties */


static void* signal_thread(void *arg) {
    int r;

    /* add signal matches here */
    r = sd_bus_match_signal(bus,
                            NULL,
                            NULL,
                            OBJECT_PATH,
                            INTERFACE_NAME,
                            "StatusSignal", 
                            status_signal_cb, 
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

    
    /* wait for interupt and loop */
    while(endProgram == 0) {
        r = sd_bus_process(bus, NULL);
        dbusError(r, "Bus process failed.");
        if(r > 0)
            continue;

        r = sd_bus_wait(bus, (uint64_t)-1);
        dbusError(r, "Bus wait failed.");
    }

    return NULL;
}


/* callback for reading the status signal form the Updater */
static int status_signal_cb(sd_bus_message *m, void *user_data, sd_bus_error *ret_error) {
    int r;
    int32_t state;

    r = sd_bus_message_read(m, "i", &state);
    dbusError(r, "Failed to parse file status signal.");
    if (r > 0)
        return -1;

    OD_set(0x1203, 1, state);

    return 0;
}


/* callback for handling file transfer from the Updater */
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


/****************************************************************************/
/* methods for main dbus interface thread to call */


int Updater_allMethods() {
    update();
    /* Add other gps dbus method check funtions here */
    return 1;
}


/* Handle new state change with dbus method call to Updater process */
static void update(void) {
    int r;
    sd_bus_error error = SD_BUS_ERROR_NULL;
    sd_bus_message *m = NULL;
    int32_t current_state = OD_get_i32(0x1203, 1);
    int32_t new_state = OD_get_i32(0x1202, 1);

    if(new_state == current_state)
        return; /* no need to change */

    /* Issue the method call and store the response message in m */
    r = sd_bus_call_method(bus,
                           BUS_NAME,
                           OBJECT_PATH,
                           INTERFACE_NAME,
                           "Update",
                           &error,
                           NULL,
                           "n",
                           new_state);
    dbusError(r, "Failed to issue method call.");

    sd_bus_error_free(&error);
    sd_bus_message_unref(m);
    return;
}


