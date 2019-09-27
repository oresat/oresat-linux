/*
 * Application interface for CANopenSocket.
 *
 * @file        application.c
 * @author      Janez Paternoster
 * @copyright   2016 Janez Paternoster
 *
 * This file is part of CANopenSocket, a Linux implementation of CANopen
 * stack with master functionality. Project home page is
 * <https://github.com/CANopenNode/CANopenSocket>. CANopenSocket is based
 * on CANopenNode: <https://github.com/CANopenNode/CANopenNode>.
 *
 * CANopenSocket is free and open source software: you can redistribute
 * it and/or modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation, either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <systemd/sd-bus.h>
#include <pthread.h>


#include "CANopen.h"
#include "CO_driver.h"
#include "dbus.h"
#include "application.h"
#ifdef GPS_INTERFACE
#include "GPS_interface.h"
#endif
#ifdef ST_INTERFACE
#include "ST_interface.h"
#endif
#ifdef UPDATER_INTERFACE
#include "Updaer_interface.h"
#endif

/* Maximum size of Object Dictionary variable transmitted via SDO. */
#ifndef CO_COMMAND_SDO_BUFFER_SIZE
#define CO_COMMAND_SDO_BUFFER_SIZE     1000000
#endif


/* Static Variables */
static CO_OD_file_data_t    odSendFileData;     /* Pointer to file data for DOMAIN based OD entry */
static CO_OD_file_data_t    odReceiveFileData;  /* Pointer to file data for DOMAIN based OD entry */


/******************************************************************************/
void app_programStart(void){
    /* initialize Object Dictionary file data */
    CO_OD_file_transfer_init(&odSendFileData);
    CO_OD_file_transfer_init(&odReceiveFileData);

    /* Add data structs to SDO Object Dictionary */
    CO_OD_configure(CO->SDO[0], 0x3002, file_transfer, (void*)&odSendFileData, 0, 0U);
    CO_OD_configure(CO->SDO[0], 0x3003, file_transfer, (void*)&odReceiveFileData, 0, 0U);

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

    return;
}


/******************************************************************************/
void app_communicationReset(void){

}


/******************************************************************************/
void app_programEnd(void){
    /* Handle dynamic memory for file data */
    CO_OD_file_transfer_close(&odSendFileData);
    CO_OD_file_transfer_close(&odReceiveFileData);

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

    return;
}


/******************************************************************************/
void app_programAsync(uint16_t timer1msDiff){

}


/******************************************************************************/
void app_program1ms(void){

}


/******************************************************************************/
CO_SDO_abortCode_t file_transfer(CO_ODF_arg_t *ODF_arg) {
    printf("File transfer started.%d\n", ODF_arg->offset);

    CO_OD_file_data_t *odFileData;
    CO_SDO_abortCode_t ret = CO_SDO_AB_NONE;

    odFileData = (CO_OD_file_data_t*) ODF_arg->object;

    if(ODF_arg->subIndex != 2) {
        printf("wrong subidx\n");
        // nothing special
        ret = CO_SDO_AB_NO_DATA; 
        return ret;
    }

    printf("RW %d\n", ODF_arg->reading);
    if(ODF_arg->reading) { 
        printf("Reading\n");
        /* read parameters */
        if(odFileData->fileData == NULL || odFileData->fileSize == 0) {
            //error, no data to read
            printf("No data\n\n");
            ret = CO_SDO_AB_NO_DATA; 
            return ret;
        }

        if(ODF_arg->firstSegment) {
            printf("start reading 1st seg\n");
            /* 1st offset */
            ODF_arg->dataLengthTotal = odFileData->fileSize;
            ODF_arg->firstSegment = 0;
        }

        if(ODF_arg->dataLength < CO_COMMAND_SDO_BUFFER_SIZE)
            ODF_arg->lastSegment = 1;

        memcpy(ODF_arg->data, &(odFileData->fileData[ODF_arg->offset]), ODF_arg->dataLength);
        ++ODF_arg->offset;
    }
    else { 
        printf("Writing\n");
        /* store parameters */
        if(ODF_arg->firstSegment) {
            /* 1st segment */
            if(odFileData->fileData != NULL) {
                /* clear data for new transfer */
                free(odFileData->fileData);
                printf("clearing data\n");
            }
            printf("Allocating data\n");

            /* figure out data length */
            if(ODF_arg->dataLengthTotal > 0) {
                /* dataLengthTotal was set */
                odFileData->fileSize = ODF_arg->dataLengthTotal;
            }
            else {
                /* dataLengthTotal was not set */
                odFileData->fileSize = ODF_arg->dataLength;
                ODF_arg->dataLengthTotal = ODF_arg->dataLength; 
            }

            /* allocate memory for new file */
            odFileData->fileData = (uint8_t *)malloc(odFileData->fileSize);
            ODF_arg->firstSegment = 0;
            printf("Data size %d\n", ODF_arg->dataLengthTotal);
        }
        
        if(ODF_arg->dataLength < CO_COMMAND_SDO_BUFFER_SIZE)
            ODF_arg->lastSegment = 1;

        memcpy(&(odFileData->fileData[ODF_arg->offset]), ODF_arg->data, ODF_arg->dataLength);
        ++ODF_arg->offset;
    }

    printf("\n");
    return ret;
}


CO_ReturnError_t CO_OD_file_transfer_init(CO_OD_file_data_t *odFileData) {
    odFileData->fileData = NULL;
    odFileData->fileSize = 0;
    return CO_ERROR_NO;
}


void CO_OD_file_transfer_close(CO_OD_file_data_t *odFileData) {
    if(odFileData->fileData != NULL) {
        free(odFileData->fileData);
        odFileData = NULL;
    }
    return;
}
