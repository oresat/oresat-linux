/**
 *
 */

#include "updaer.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <systemd/sd-bus.h>


#define INTERFACE_NAME  "org.OreSat.Updater"
#define BUS_NAME        INTERFACE_NAME
#define OBJECT_PATH     "/org/OreSat/Updater"
#define FILE_NAME_SIZE


/* Static Variables */
static volatile int endProgram = 0;
static sd_bus *bus = NULL;
static uint8_t current_state = 0;
static char current_file[FILE_NAME_SIZE] = "\0";


/* Static Functions */


/******************************************************************************/
void dbusError(int r, char* err) {
    if (r < 0)
        fprintf(stderr, "%s %s\n", err, strerror(-r));
    return;
}


void dbusErrorExit(int r, char* err) {
    if (r < 0) {
        fprintf(stderr, "%s %s\n", err, strerror(-r));
        exit(0);
    }
    return;
}


/******************************************************************************/
void updater_programStart(void){
    int r;

    /* Connect to the bus */
    r = sd_bus_open_system(&bus);
    dbusError(r, "Failed to connect to system bus:");

    CO_OD_configure(CO->SDO[0], 0x3100, CB_ODF_3100, NULL, 0, 0U);

    return;
}


/******************************************************************************/
void updater_communicationReset(void){

}


/******************************************************************************/
void updater_programEnd(void){
    sd_bus_error_free(&error);
    sd_bus_unref(bus);

    return;
}


/******************************************************************************/
void updater_programAsync(uint16_t timer1msDiff){

}


/******************************************************************************/
void updater_program1ms(void){

}


/******************************************************************************/


CO_SDO_abortCode_t CB_ODF_3002(CO_ODF_arg_t *ODF_arg) {
    file_buffer_t *odFileBuffer;
    CO_SDO_abortCode_t ret = CO_SDO_AB_NONE;
    sd_bus_error err = SD_BUS_ERROR_NULL;
    sd_bus_message *mess;
    int r;

    APP_LOCK_ODF();

    switch(ODF_arg->subIndex) {
        case 1 : /* current state */
            
            if(ODF_arg->reading == true) {
                ODF_arg->dataLength = sizeof(current_state);
                memcpy(ODF_arg->data, &current_state, ODF_arg->dataLength);
            }
            else
                ret = CO_SDO_AB_READONLY; /* can't write parameters, read only */
            
            break;

        case 2 : /* current file */

            if(ODF_arg->reading == true)  {
                ODF_arg->dataLength = strlen(current_file);
                memcpy(ODF_arg->data, current_file, ODF_arg->dataLength);
            }
            else
                ret = CO_SDO_AB_READONLY; /* can't write parameters, read only */
            
            break;

        case 3 : /* give updater new file, will not update with it yet */
            
            if(ODF_arg->reading == true)
                ret = CO_SDO_AB_WRITEONLY; /* can't read parameters, write only */
            else {
                //TODO copy file into local directory
            }

            break;

        case 4 : /* number of updates avail */
            
            if(ODF_arg->reading == true) 
                ODF_arg->dataLength = sizeof(updates_available);
                memcpy(ODF_arg->data, &updates_available, ODF_arg->dataLength);
            else
                ret = CO_SDO_AB_READONLY; /* can't write parameters, read only */
            
            break;

        case 5 : /* update */
            
            if(ODF_arg->reading == true)
                ret = CO_SDO_AB_WRITEONLY; /* can't read parameters, write only */
            else {
                r = sd_bus_call_method(bus,
                                       BUS_NAME,
                                       OBJECT_PATH,
                                       INTERFACE_NAME,
                                       "Update",
                                       &error,
                                       &mess,
                                       "d",
                                       temp);
                dbusError(r, "Failed to issue method call:");

                /* Parse the response message */
                r = sd_bus_message_read(mess, "d", &temp);
                dbusError(r, "Failed to parse response message:");

                sd_bus_message_unref(mess);
            }

            break;

        case 6 : /* stop update */
            int32_t temp;

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
                                       "d",
                                       temp);
                dbusError(r, "Failed to issue method call:");

                /* Parse the response message */
                r = sd_bus_message_read(mess, "d", &temp);
                dbusError(r, "Failed to parse response message:");
                
                sd_bus_message_unref(mess);
                sd_bus_error_free(&err);
            }

            break;

        case 7 : /* delete current file */

            if(ODF_arg->reading == true)
                ret = CO_SDO_AB_WRITEONLY; /* can't read parameters, write only */
            else {
                //TODO call stop update method
            }

            break;

        default :
            ret = CO_SDO_AB_SUB_UNKNOWN; 
    }

    ODF_arg->lastSegment = true;

    APP_UNLOCK_ODF();

    return ret;
}

