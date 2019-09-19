/*
 * Main Process interface.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <systemd/sd-bus.h>
#include <pthread.h>

#include "GPS_interface.h"
#include "dbus_helpers.h"

#define INTERFACE_NAME  "org.OreSat.GPS"
#define BUS_NAME        INTERFACE_NAME
#define OBJECT_PATH     "/org/OreSat/GPS"
#define SIGNAL_THREAD_STACK_SIZE CO_COMMAND_SDO_BUFFER_SIZE*3

/* Static Variables */
static pthread_t        signal_thread_id;
static pthread_attr_t   signal_thread_attr;
static volatile int     endProgram = 0;
static sd_bus           *bus = NULL;

/* Static Functions */
static void* signal_thread(void *arg);
static int status_signal_cb(sd_bus_message *, void *, sd_bus_error *);
static int data_signal_cb(sd_bus_message *, void *, sd_bus_error *);
static void updateState(void);


int GPS_interface_init(void) {
    int r;
    endProgram = 0;

    /* Connect to the system bus */
    r = sd_bus_open_system(&bus);
    dbusErrorExit(r, "Failed to connect to system bus.");

    pthread_attr_setstacksize(&signal_thread_attr, SIGNAL_THREAD_STACK_SIZE);
    r = pthread_create(&signal_thread_id, &signal_thread_attr, signal_thread, NULL);
    dbusError(-r, "dbus_init - signal thread creation failed");
    
    return r;
}


int GPS_interface_clear(void) {
    endProgram = 1;

    /* Wait for threads to finish. */
    if(pthread_join(signal_thread_id, NULL) != 0) {
        return -1;
    }

    sd_bus_unref(bus);

    return 0;
}


/****************************************************************************/
/* singals / properties */


static void* signal_thread(void *arg) {
    sd_bus_slot *slot = NULL;
    int r;

    /* add signal matches here */
    /*
    r = sd_bus_add_match(bus,
                         slot,
                        "type='signal', path='"OBJECT_PATH"', interface='"INTERFACE_NAME"', member='StatusSignal'", 
                         status_signal_cb, 
                         NULL);
    dbusErrorExit(r, "Add match error for status signal.");
    */

    r = sd_bus_add_match(bus,
                         slot,
                        "type='signal', path='"OBJECT_PATH"', interface='"INTERFACE_NAME"', member='StateVectorSignal'",
                         data_signal_cb, 
                         NULL);
    dbusErrorExit(r, "Add match error for state vector signal.");

    
    /* wait for interupt and loop */
    while(endProgram == 0) {
        r = sd_bus_process(bus, NULL);
        dbusError(r, "Bus process failed.");
        if(r > 0)
            continue;

        r = sd_bus_wait(bus, (uint64_t)-1);
        dbusError(r, "Bus wait failed.");
    }

    sd_bus_slot_unref(slot);
    return NULL;
}


/* callback for reading the status signal form the GPS */
static int status_signal_cb(sd_bus_message *m, void *user_data, sd_bus_error *ret_error) {
    int r;
    int32_t state;

    r = sd_bus_message_read(m, "i", &state);
    dbusError(r, "Failed to parse status signal.");
    if (r > 0)
        return -1;

    OD_setData(0x3002, 1, &state, sizeof(state));

    return 0;
}


/* callback for reading the data signal from the GPS */
static int data_signal_cb(sd_bus_message *m, void *user_data, sd_bus_error *ret_error) {
    int r;
    int16_t posX = 0;
    int16_t posY = 0;
    int16_t posZ = 0;
    /*
    int16_t velX = 0;
    int16_t velY = 0;
    int16_t velZ = 0;
    int16_t accX = 0;
    int16_t accY = 0;
    int16_t accZ = 0;*/

    r = sd_bus_message_read(m, "nnn", &posX, &posY, &posZ); //, &velX, &velY, &velZ, &accX, &accY, &accZ);
    dbusError(r, "Failed to parse data signal.");

    if (r > 0)
        return -1;

    fprintf(stderr, "%d %d %d\n", posX, posY, posZ);

    OD_setData(0x3101, 1, &posX, sizeof(posX));
    OD_setData(0x3101, 2, &posY, sizeof(posY));
    OD_setData(0x3101, 3, &posZ, sizeof(posZ));/*
    OD_setData(0x3101, 4, &velX, sizeof(velX));
    OD_setData(0x3101, 5, &velY, sizeof(velY));
    OD_setData(0x3101, 6, &velZ, sizeof(velZ));
    OD_setData(0x3101, 7, &accX, sizeof(accX));
    OD_setData(0x3101, 8, &accY, sizeof(accY));
    OD_setData(0x3101, 9, &accZ, sizeof(accZ));*/
    dbusErrorExit(-1, "State vector.");

    return 0;
}


/****************************************************************************/
/* methods for main dbus interface thread to call */


int GPS_allMethods() {
    updateState();
    /* Add other gps dbus method check funtions here */
    return 1;
}


/* Handle new state change with dbus method call to GPS process */
static void updateState(void) {
    int r;
    sd_bus_error error = SD_BUS_ERROR_NULL;
    sd_bus_message *m = NULL;
    int32_t current_state, new_state, return_int;

    OD_getNonArrayData(0x3000, 1, &new_state, sizeof(new_state));
    OD_getNonArrayData(0x3100, 1, &current_state, sizeof(current_state));

    if(new_state == current_state)
        return; /* no need to change */

    /* Issue the method call and store the response message in m */
    r = sd_bus_call_method(bus,
                           BUS_NAME,
                           OBJECT_PATH,
                           INTERFACE_NAME,
                           "ChangeState",
                           &error,
                           &m,
                           "i",
                           new_state);
    dbusError(r, "Failed to issue method call.");

    /* Parse the response message */
    r = sd_bus_message_read(m, "i", &return_int);
    dbusError(r, "Failed to parse response message.");

    sd_bus_error_free(&error);
    return;
}


