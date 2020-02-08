#include "CANopen.h"
#include "CO_SDO.h"
#include "OD_helpers.h"
#include "file_transfer_ODF.h"
#include "log_message.h"
#include "updater.h"
#include <systemd/sd-bus.h>
#include <pthread.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <pthread.h>


#define DESTINATION         "org.oresat.updater"
#define INTERFACE_NAME      "org.oresat.updater"
#define OBJECT_PATH         "/org/oresat/updater"
#define FILE_NAME_SIZE      100
#define ERROR_MESSAGE_SIZE  100


// Static variables
static sd_bus               *bus = NULL;
static pthread_t            signal_thread_id;
static bool                 end_program = false;
static int32_t              current_state = 0;
static uint32_t             updates_available = 0;
static char                 current_file[FILE_NAME_SIZE] = "\0";
static char                 error_message[ERROR_MESSAGE_SIZE] = "\0";


// Static functions headers
static int read_status_cb(sd_bus_message *m, void *userdata, sd_bus_error *ret_error);
static void* signal_thread(void* arg);


// ***************************************************************************
// updater dbus functions


int updater_dbus_setup(void) {
    int r;
    void* userdata = NULL;

    // add updater_ODF to OD
    CO_OD_configure(CO->SDO[0], 0x3004, updater_ODF, NULL, 0, 0U);

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
            read_status_cb, 
            userdata);
    if (r < 0) {
        log_message(LOG_ERR, "Failed to add new signal match.\n");
        return r;
    }

    // Start dbus signal thread
    if (pthread_create(&signal_thread_id, NULL, signal_thread, NULL) != 0) {
        log_message(LOG_DEBUG, "Failed to start dbus signal thread.\n");
        return -1;
    }

    return 0;
}


int updater_dbus_end(void) {

    // stop dbus signal thread
    end_program = true;

    struct timespec tim;
    tim.tv_sec = 1;
    tim.tv_nsec = 0;

    if (nanosleep(&tim, NULL) < 0 ) {
        log_message(LOG_DEBUG, "Nano sleep system call failed \n");
    }

    if (pthread_join(signal_thread_id, NULL) != 0) {
        log_message(LOG_DEBUG, "updater signal thread join failed.\n");
        return -1;
    }

    sd_bus_unref(bus);
    return 0;
}


// ***************************************************************************
// updater dbus signal functions


static int read_status_cb(sd_bus_message *m, void *userdata, sd_bus_error *ret_error) {
    int r;

    r = sd_bus_get_property(
            bus, 
            DESTINATION, 
            OBJECT_PATH, 
            INTERFACE_NAME, 
            "Status",  
            ret_error, 
            &m, 
            "i");
    if (r < 0)
        return r;

    r = sd_bus_message_read(m, "i", &current_state);
    if (r < 0)
        return r;
    
    r = sd_bus_get_property(
            bus, 
            DESTINATION, 
            OBJECT_PATH, 
            INTERFACE_NAME, 
            "CurrentUpdateFile",  
            ret_error, 
            &m, 
            "s");
    if (r < 0)
        return r;

    r = sd_bus_message_read(m, "s", &current_file);
    if (r < 0)
        return r;

    return 0;
}


static void* signal_thread(void* arg) {
    int r;
    sd_bus_error err = SD_BUS_ERROR_NULL;

    while (end_program == false) {
        // Process requests
        r = sd_bus_process(bus, NULL);
        if ( r < 0) 
            log_message(LOG_DEBUG, "Failed to processA bus.\n");
        else if (r > 0) // we processed a request, try to process another one, right-away
            continue;

        // Wait for the next request to process 
        if (sd_bus_wait(bus, 100000) < 0)
            log_message(LOG_DEBUG, "Bus wait failed.\n");
    }
    printf("app thread exited \n");

    sd_bus_error_free(&err);
    return NULL;
}


// ***************************************************************************
// updater ODF(s)


CO_SDO_abortCode_t updater_ODF(CO_ODF_arg_t *ODF_arg) {
    CO_SDO_abortCode_t ret = CO_SDO_AB_NONE;
    sd_bus_error err = SD_BUS_ERROR_NULL;
    sd_bus_message *mess;
    int r;
    bool_t temp_bool;

    switch(ODF_arg->subIndex) {
        case 1 : /* current state */
            
            if(ODF_arg->reading == true) {
                ODF_arg->dataLength = sizeof(current_state);
                memcpy(ODF_arg->data, &current_state, ODF_arg->dataLength);
            }
            else
                ret = CO_SDO_AB_READONLY; /* can't write parameters, read only */
            
            break;

        case 2 : /* updates available */
            
            if(ODF_arg->reading == true) {
                ODF_arg->dataLength = sizeof(updates_available);
                memcpy(ODF_arg->data, &updates_available, ODF_arg->dataLength);
            }
            else
                ret = CO_SDO_AB_READONLY; /* can't write parameters, read only */
            
            break;

        case 3 : /* current file */

            if(ODF_arg->reading == true)  {
                ODF_arg->dataLength = strlen(current_file);
                memcpy(ODF_arg->data, current_file, ODF_arg->dataLength);
            }
            else
                ret = CO_SDO_AB_READONLY; /* can't write parameters, read only */
            
            break;

        case 4 : /* error message  */

            if(ODF_arg->reading == true)  {
                ODF_arg->dataLength = strlen(error_message);
                memcpy(ODF_arg->data, error_message, ODF_arg->dataLength);
            }
            else
                ret = CO_SDO_AB_READONLY; /* can't write parameters, read only */
            
            break;

        case 5 : /* give updater new file, will not update with it yet */
            
            if(ODF_arg->reading == true)
                ret = CO_SDO_AB_WRITEONLY; /* can't read parameters, write only */
            else {
                /* dbus interface not up */
                if(bus == NULL) {
                    ret = CO_SDO_AB_GENERAL;
                    break;
                }

                if(ODF_arg->dataLength > FILE_PATH_MAX_LENGTH) {
                    ret = CO_SDO_AB_GENERAL; /* file path to big */
                    break;
                }

                /* copy file name into a temp var */
                char new_update_file[FILE_PATH_MAX_LENGTH] = "\0";
                memcpy(new_update_file, ODF_arg->data, ODF_arg->dataLength);

                /* make sure its an absoult path */
                if(new_update_file[0] != '/') {
                    ret = CO_SDO_AB_GENERAL;
                    break;
                }

                r = sd_bus_call_method(
                        bus,
                        DESTINATION,
                        OBJECT_PATH,
                        INTERFACE_NAME,
                        "AddUpdateFile",
                        &err,
                        &mess,
                        "s",
                        new_update_file);
                if (r < 0)
                    log_message(LOG_DEBUG, "Failed to issue method call.");

                /* Parse the response message */
                r = sd_bus_message_read(mess, "b", &temp_bool);
                if (r < 0)
                    log_message(LOG_DEBUG, "Failed to parse response message.");

                sd_bus_message_unref(mess);
                sd_bus_error_free(&err);
            }

            break;

        case 6 : /* start update */
            
            if(ODF_arg->reading == true)
                ret = CO_SDO_AB_WRITEONLY; /* can't read parameters, write only */
            else {
                r = sd_bus_call_method(
                        bus,
                        DESTINATION,
                        OBJECT_PATH,
                        INTERFACE_NAME,
                        "StartUpdate",
                        &err,
                        &mess,
                        NULL);
                if (r < 0)
                    log_message(LOG_DEBUG, "Failed to issue method call.");

                /* Parse the response message */
                r = sd_bus_message_read(mess, "b", &temp_bool);
                if (r < 0)
                    log_message(LOG_DEBUG, "Failed to parse response message.");

                sd_bus_message_unref(mess);
                sd_bus_error_free(&err);
            }

            break;

        case 7 : /* emergency stop update */

            if(ODF_arg->reading == true)
                ret = CO_SDO_AB_WRITEONLY; /* can't read parameters, write only */
            else {
                r = sd_bus_call_method(
                        bus,
                        DESTINATION,
                        OBJECT_PATH,
                        INTERFACE_NAME,
                        "StopUpdate",
                        &err,
                        &mess,
                        NULL);
                if (r < 0)
                    log_message(LOG_DEBUG, "Failed to issue method call.");

                /* Parse the response message */
                r = sd_bus_message_read(mess, "b", &temp_bool);
                if (r < 0)
                    log_message(LOG_DEBUG, "Failed to parse response message.");
                
                sd_bus_message_unref(mess);
                sd_bus_error_free(&err);
            }

            break;

        case 8 : /* reset linux updater */

            if(ODF_arg->reading == true)
                ret = CO_SDO_AB_WRITEONLY; /* can't read parameters, write only */
            else {
                r = sd_bus_call_method(
                        bus,
                        DESTINATION,
                        OBJECT_PATH,
                        INTERFACE_NAME,
                        "Reset",
                        &err,
                        &mess,
                        NULL);
                if (r < 0)
                    log_message(LOG_DEBUG, "Failed to issue method call.");

                /* Parse the response message */
                r = sd_bus_message_read(mess, "b", &temp_bool);
                if (r < 0)
                    log_message(LOG_DEBUG, "Failed to parse response message.");

                sd_bus_message_unref(mess);
                sd_bus_error_free(&err);
            }

            break;

        case 9 : /* get apt update output as a file */

            if(ODF_arg->reading == true)
                ret = CO_SDO_AB_WRITEONLY; /* can't read parameters, write only */
            else {
                r = sd_bus_call_method(
                        bus,
                        DESTINATION,
                        OBJECT_PATH,
                        INTERFACE_NAME,
                        "GetAptListOutput",
                        &err,
                        &mess,
                        NULL);
                if (r < 0)
                    log_message(LOG_DEBUG, "Failed to issue method call.");

                /* Parse the response message */
                r = sd_bus_message_read(mess, "b", &temp_bool);
                if (r < 0)
                    log_message(LOG_DEBUG, "Failed to parse response message.");

                sd_bus_message_unref(mess);
                sd_bus_error_free(&err);
            }

            break;

        default :
            ret = CO_SDO_AB_SUB_UNKNOWN; 
    }

    ODF_arg->lastSegment = true;

    return ret;
}

