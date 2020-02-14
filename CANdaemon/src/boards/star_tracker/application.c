#include "application.h"
#include "OD_helpers.h"
#include "file_transfer_ODF.h"
#include "log_message.h"
#include <systemd/sd-bus.h>
#include <pthread.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>


#define DESTINATION     "org.oresat.startracker"
#define INTERFACE_NAME  "org.oresat.startracker"
#define OBJECT_PATH     "/org/oresat/startracker"


// Static variables
static sd_bus           *bus = NULL;
static pthread_t        app_signal_thread_id;
static bool             end_program = false;


// Static functions headers
//static int read_orientation_cb(sd_bus_message *m, void *userdata, sd_bus_error *ret_error);
//static void* app_signal_thread(void* arg);


// ***************************************************************************
// app dbus functions


int app_dbus_setup(void) {
    int r;

    /* Connect to the bus */
    r = sd_bus_open_system(&bus);
    if (r < 0) {
        log_message(LOG_ERR, "Failed to connect to systemd bus.\n");
        return r;
    }

    CO_OD_configure(CO->SDO[0], 0x3100, test_camera_ODF, NULL, 0, 0U);

    return 0;
}


int app_dbus_end(void) {
    sd_bus_unref(bus);
    return 0;
}


// ***************************************************************************
// other cirrus flux camera functions


CO_SDO_abortCode_t test_camera_ODF(CO_ODF_arg_t *ODF_arg) {
    CO_SDO_abortCode_t ret = CO_SDO_AB_NONE;
    sd_bus_error err = SD_BUS_ERROR_NULL;
    sd_bus_message *m = NULL;
    uint8_t temp = 0;
    char *file_path;
    int r;

    if(ODF_arg->reading == false) { /* write parameters */
        ODF_arg->dataLength = sizeof(temp);
        memcpy(ODF_arg->data, &temp, ODF_arg->dataLength);
    }

    /* Issue the method call and store the response message in m */
    r = sd_bus_call_method(
            bus,
            DESTINATION,
            OBJECT_PATH,
            INTERFACE_NAME,
            "LatestImage",
            &err,
            &m,
            NULL);
    if (r < 0) {
        log_message(LOG_DEBUG, "Failed to issue LatestImage method call.");
        return CO_SDO_AB_GENERAL;
    }

    /* Parse the response message */
    r = sd_bus_message_read(m, "s", &file_path);
    if (r < 0) {
        log_message(LOG_DEBUG, "Failed to parse response message from LatestImage.");
        return CO_SDO_AB_GENERAL;
    }

    if(file_path != NULL)
        app_send_file(file_path);

    sd_bus_error_free(&err);
    sd_bus_message_unref(m);

    ODF_arg->lastSegment = true;

    return ret;
}


/*

int app_dbus_setup(void) {
    int r;
    void* userdata = NULL;

    // Connect to the bus
    r = sd_bus_open_system(&bus);
    if (r < 0) {
        log_message(LOG_ERR, "Failed to connect to systemd bus.\n");
        return r;
    }

    r = sd_bus_match_signal(
            bus,
            NULL,
            NULL,
            OBJECT_PATH,
            "org.freedesktop.DBus.Properties",
            "PropertiesChanged", 
            read_orientation_cb, 
            userdata);
    if (r < 0) {
        log_message(LOG_ERR, "Failed to add new signal match.\n");
        return r;
    }

    //start dbus signal thread
    if (pthread_create(&app_signal_thread_id, NULL, app_signal_thread, NULL) != 0) {
        log_message(LOG_ERR, "Failed to start dbus signal thread.\n");
        return -1;
    }

    return 0;
}


int app_dbus_end(void) {

    // stop dbus signal thread
    end_program = true;

    struct timespec tim;
    tim.tv_sec = 1;
    tim.tv_nsec = 0;

    if (nanosleep(&tim, NULL) < 0 ) {
        log_message(LOG_DEBUG, "Nano sleep system call failed \n");
    }

    if (pthread_join(app_signal_thread_id, NULL) != 0) {
        log_message(LOG_DEBUG, "app signal thread join failed.\n");
        return -1;
    }

    sd_bus_unref(bus);
    return 0;
}


// ***************************************************************************
// other star tracker functions


static int read_orientation_cb(sd_bus_message *m, void *userdata, sd_bus_error *ret_error) {
    double declination, right_ascension, orientation = 0.0;

    if (sd_bus_get_property(bus, DESTINATION, OBJECT_PATH, INTERFACE_NAME, "DEC", ret_error, &m, "ddd") < 0)
        return 1; // failed to get property
    
    if (sd_bus_message_read(m, "ddd", &declination, &right_ascension, &orientation) < 0)
        return 1; // failed to decode dbus property

    // update OD
    app_writeOD(0x3101, 1, &declination, sizeof(declination));
    app_writeOD(0x3101, 2, &right_ascension, sizeof(right_ascension));
    app_writeOD(0x3101, 3, &orientation, sizeof(orientation));

    return 0;
}


static void* app_signal_thread(void* arg) {
    int r;
    sd_bus_error err = SD_BUS_ERROR_NULL;

    while (end_program == false) {
        // Process requests
        r = sd_bus_process(bus, NULL);
        if ( r < 0) 
            log_message(LOG_DEBUG, "Failed to process bus.\n");
        else if (r > 0) // we processed a request, try to process another one, right-away
            continue;

        // Wait for the next request to process 
        if (sd_bus_wait(bus, 100000) < 0)
            log_message(LOG_DEBUG, "Bus wait failed.\n");
    }

    sd_bus_error_free(&err);
    return NULL;
}
*/
