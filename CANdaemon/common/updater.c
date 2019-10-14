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
#define SIGNAL_THREAD_STACK_SIZE FILE_TRANSFER_MAX_SIZE*3


/* Static Variables */
static volatile int     endProgram = 0;
static sd_bus          *bus = NULL;
static sd_bus_error     error = SD_BUS_ERROR_NULL;


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
    uint8_t temp = 0;

    APP_LOCK_ODF();

    if(ODF_arg->subIndex == 1) { /* get latest image */
        if(ODF_arg->reading == false) { /* read parameters */
            ODF_arg->dataLength = sizeof(temp);
            memcpy(ODF_arg->data, &temp, ODF_arg->dataLength);
        }
        else {
            /* Issue the method call and store the response message in m */
            r = sd_bus_call_method(bus,
                                   BUS_NAME,
                                   OBJECT_PATH,
                                   INTERFACE_NAME,
                                   "Update",
                                   &error,
                                   &m,
                                   "d",
                                   temp);
            dbusError(r, "Failed to issue method call:");

            /* Parse the response message */
            r = sd_bus_message_read(m, "d", &temp);
            dbusError(r, "Failed to parse response message:");

            sd_bus_message_unref(m);
        }

    }
    else 
        ret = CO_SDO_AB_SUB_UNKNOWN; 

    ODF_arg->lastSegment = true;

    APP_UNLOCK_ODF();

    return ret;
}

