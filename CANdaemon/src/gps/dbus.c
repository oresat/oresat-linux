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
#define METHOD_THREAD_DELAY 1000000

/* Variables */
static pthread_t        signal_thread_id;
static pthread_t        method_thread_id;
static pthread_attr_t   signal_thread_attr;
static volatile int     endProgram = 0;
static sd_bus           *bus = NULL;

/* static functions */
static void* signal_thread(void *);
static void* method_thread(void *);
static void MP_update_state(void);
static int file_transfer_signal_cb(sd_bus_message *, void *, sd_bus_error *);
static int data_signal_cb(sd_bus_message *, void *, sd_bus_error *);


int dbus_init(void) {
    int r;

    if(CO == NULL || CO->SDOclient == NULL){
        CO_errExit("dbus_init - Wrong arguments");
    }

    /* Connect to the user bus */
    r = sd_bus_open_system(&bus);
    dbusErrorExit(r, "Failed to connect to system bus.");

    /* Create thread */
    endProgram = 0;
    pthread_attr_setstacksize(&signal_thread_attr, SIGNAL_THREAD_STACK_SIZE);
    if(pthread_create(&signal_thread_id, &signal_thread_attr, signal_thread, NULL) != 0) {
        CO_errExit("dbus_init - signal thread creation failed");
    }
    if(pthread_create(&method_thread_id, NULL, method_thread, NULL) != 0) {
        CO_errExit("dbus_init - method thread creation failed");
    }

    return 0;
}


int dbus_clear(void) {
    endProgram = 1;

    /* Wait for thread to finish. */
    if(pthread_join(signal_thread_id, NULL) != 0) {
        return -1;
    }
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

    /* add signal matches */
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

    return NULL;
}

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
    int16_t posX = 0;
    int16_t posY = 0;
    int16_t posZ = 0;

    r = sd_bus_message_read(m, "nnn", &posX, &posY, &posZ);
    dbusError(r, "Failed to parse data signal.");

    if (r > 0)
        return -1;

    fprintf(stderr, "%d %d %d\n", posX, posY, posZ);

    OD_set(0x3000, 1, posX);
    OD_set(0x3000, 2, posY);
    OD_set(0x3000, 3, posZ);

    return 0;
}


/****************************************************************************/
/* methods */


static void* method_thread(void *arg) {
    while(endProgram == 0) {
        MP_update_state();
        usleep(METHOD_THREAD_DELAY);
    }

    return NULL;
}


static void MP_update_state(void) {
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
                           "ChangeState",
                           &error,
                           NULL,
                           "n",
                           new_state);
    dbusError(r, "Failed to issue method call.");

    sd_bus_error_free(&error);
    sd_bus_message_unref(m);
    return;
}


