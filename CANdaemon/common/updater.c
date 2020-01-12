/**
 *
 */

#include "CANopen.h"
#include "CO_driver.h"
#include "updater.h"
#include "error_assert_handlers.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <systemd/sd-bus.h>
#include <signal.h>


#define INTERFACE_NAME  "org.OreSat.Updater"
#define BUS_NAME        INTERFACE_NAME
#define OBJECT_PATH     "/org/OreSat/Updater"
#define FILE_NAME_SIZE 100
#define ERROR_MESSAGE_SIZE 100


/* Static Variables */
static volatile int endProgram = 0;
static sd_bus *bus = NULL;
static int32_t current_state = 0;
static uint32_t updates_available = 0;
static char current_file[FILE_NAME_SIZE] = "\0";
static char error_message[ERROR_MESSAGE_SIZE] = "\0";


/* Static Functions */


/******************************************************************************/
void updater_programStart(void){
    sd_bus_error err = SD_BUS_ERROR_NULL;
    int r;

    /* Connect to the bus */
    r = sd_bus_open_system(&bus);

    /* set computer name on linux updater */
    char computer_name[] = "StarTracker"; // TODO get computer_name
    r = sd_bus_set_property(bus, 
                            BUS_NAME, 
                            OBJECT_PATH, 
                            INTERFACE_NAME, 
                            "ComputerName", 
                            &err, 
                            "s", 
                            computer_name);

    sd_bus_error_free(&err);

    CO_OD_configure(CO->SDO[0], 0x3004, CO_ODF_3004, NULL, 0, 0U);

    return;
}


/******************************************************************************/
void updater_communicationReset(void){
    return;
}


/******************************************************************************/
void updater_programEnd(void){
    return;
}


/******************************************************************************/
void updater_programAsync(uint16_t timer1msDiff){
    return;
}


/******************************************************************************/
void updater_program1ms(void){
    sd_bus_error err = SD_BUS_ERROR_NULL;
    sd_bus_message *mess = NULL;
    int r;

     /* decode dbus property */
    r = sd_bus_get_property(bus, BUS_NAME, OBJECT_PATH, INTERFACE_NAME, "Status",  &err, &mess, "i");
    if(r < 0) 
        /* since this is the 1st dbus call and it returned an error, 
         * there is no reason to call anything else */
        goto end;

    r = sd_bus_message_read(mess, "i", &current_state);
    
    if(mess != NULL)
        sd_bus_message_unref(mess);
    sd_bus_error_free(&err);

    r = sd_bus_get_property(bus, BUS_NAME, OBJECT_PATH, INTERFACE_NAME, "CurrentUpdateFile",  &err, &mess, "s");
    r = sd_bus_message_read(mess, "s", &current_state);
    
    end:

    sd_bus_error_free(&err);
    if(mess != NULL)
        sd_bus_message_unref(mess);

    return;
}


/******************************************************************************/


CO_SDO_abortCode_t CO_ODF_3004(CO_ODF_arg_t *ODF_arg) {
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

                r = sd_bus_call_method(bus,
                                       BUS_NAME,
                                       OBJECT_PATH,
                                       INTERFACE_NAME,
                                       "AddUpdateFile",
                                       &err,
                                       &mess,
                                       "s",
                                       new_update_file);

                /* Parse the response message */
                r = sd_bus_message_read(mess, "b", &temp_bool);
                dbusError(r, "Failed to parse response message:");

                sd_bus_message_unref(mess);
                sd_bus_error_free(&err);
            }

            break;

        case 6 : /* start update */
            
            if(ODF_arg->reading == true)
                ret = CO_SDO_AB_WRITEONLY; /* can't read parameters, write only */
            else {
                r = sd_bus_call_method(bus,
                                       BUS_NAME,
                                       OBJECT_PATH,
                                       INTERFACE_NAME,
                                       "StartUpdate",
                                       &err,
                                       &mess,
                                       NULL);

                /* Parse the response message */
                r = sd_bus_message_read(mess, "b", &temp_bool);
                dbusError(r, "Failed to parse response message:");

                sd_bus_message_unref(mess);
                sd_bus_error_free(&err);
            }

            break;

        case 7 : /* emergency stop update */

            if(ODF_arg->reading == true)
                ret = CO_SDO_AB_WRITEONLY; /* can't read parameters, write only */
            else {
                r = sd_bus_call_method(bus,
                                       BUS_NAME,
                                       OBJECT_PATH,
                                       INTERFACE_NAME,
                                       "StopUpdate",
                                       &err,
                                       &mess,
                                       NULL);
                dbusError(r, "Failed to issue method call:");

                /* Parse the response message */
                r = sd_bus_message_read(mess, "b", &temp_bool);
                dbusError(r, "Failed to parse response message:");
                
                sd_bus_message_unref(mess);
                sd_bus_error_free(&err);
            }

            break;

        case 8 : /* reset linux updater */

            if(ODF_arg->reading == true)
                ret = CO_SDO_AB_WRITEONLY; /* can't read parameters, write only */
            else {
                r = sd_bus_call_method(bus,
                                       BUS_NAME,
                                       OBJECT_PATH,
                                       INTERFACE_NAME,
                                       "Reset",
                                       &err,
                                       &mess,
                                       NULL);
                dbusError(r, "Failed to issue method call:");

                /* Parse the response message */
                r = sd_bus_message_read(mess, "b", &temp_bool);
                dbusError(r, "Failed to parse response message:");

                sd_bus_message_unref(mess);
                sd_bus_error_free(&err);
            }

            break;

        case 9 : /* get apt update output as a file */

            if(ODF_arg->reading == true)
                ret = CO_SDO_AB_WRITEONLY; /* can't read parameters, write only */
            else {
                r = sd_bus_call_method(bus,
                                       BUS_NAME,
                                       OBJECT_PATH,
                                       INTERFACE_NAME,
                                       "GetAptListOutput",
                                       &err,
                                       &mess,
                                       NULL);
                dbusError(r, "Failed to issue method call:");

                /* Parse the response message */
                r = sd_bus_message_read(mess, "b", &temp_bool);
                dbusError(r, "Failed to parse response message:");

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

