/*
 * DBUS
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <systemd/sd-bus.h>
#include <pthread.h>


#include "dbus.h"
#include "dbus_helpers.h"
#ifdef GPS_INTERFACE
#include "GPS_interface.h"
#endif
#ifdef ST_INTERFACE
#include "ST_interface.h"
#endif
#ifdef UPDATER_INTERFACE
#include "Updaer_interface.h"
#endif


#define METHOD_THREAD_DELAY 1000000


/* Static Variables */
static pthread_t        method_thread_id;
static int endProgram = 0;

/* Static Functions */
static void* method_thread(void *);
static void CANdaemon_allMethods(void);


int dbus_init(void) {
    int r;

    /* Create the objDict to DBus method thread */
    r = pthread_create(&method_thread_id, NULL, method_thread, NULL);
    dbusError(-r, "dbus_init - method thread creation failed");
    
    /* Create all signal/property threads */
#ifdef GPS_INTERFACE
    GPS_interface_init();
#endif
#ifdef ST_INTERFACE
    ST_interface_init();
#endif
#ifdef UPDATER_INTERFACE
    Updater_interface_init();
#endif

    return 0;
}


int dbus_clear(void) {
    endProgram = 1;

    /* Wait for threads to finish. */
    if(pthread_join(method_thread_id, NULL) != 0) {
        return -1;
    }

    /* End all signal/property threads */
#ifdef GPS_INTERFACE
    GPS_interface_clear();
#endif
#ifdef ST_INTERFACE
    ST_interface_clear();
#endif
#ifdef UPDATER_INTERFACE
    Updater_interface_clear();
#endif

    return 0;
}


static void* method_thread(void *arg) {
    uint32_t delay = METHOD_THREAD_DELAY;
    endProgram = 0;


    while(endProgram == 0) {
        usleep(delay);

        /* add all method function here as needed */
        CANdaemon_allMethods();
#ifdef GPS_INTERFACE
        GPS_allMethods();
#endif
#ifdef ST_INTERFACE
        ST_allMethods();
#endif
#ifdef UPDATER_INTERFACE
        Updater_allMethods();
#endif
    }

    return NULL;
}

static void CANdaemon_allMethods(void) {
    return;
}
