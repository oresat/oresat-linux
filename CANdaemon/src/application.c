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
    CO_OD_file_data_t *odFileData;
    CO_SDO_abortCode_t ret = CO_SDO_AB_NONE;

    odFileData = (CO_OD_file_data_t*) ODF_arg->object;

    if(ODF_arg->subIndex != 2) {
        // nothing special
        ret = CO_SDO_AB_SUB_UNKNOWN; 
        return ret;
    }

    if(ODF_arg->reading) { 
        /* read parameters */
        if(odFileData->fileSize == 0) {
            //error, no data to read
            ret = CO_SDO_AB_NO_DATA; 
            return ret;
        }

        if(ODF_arg->firstSegment) {
            /* 1st offset */
            ODF_arg->dataLength = odFileData->fileSize;
            ODF_arg->firstSegment = 0;

            if(ODF_arg->dataLength > CO_COMMAND_SDO_BUFFER_SIZE) {
                ret = CO_SDO_AB_OUT_OF_MEM; 
                return ret;
            }
            else
                ODF_arg->dataLengthTotal = odFileData->fileSize;
        }

        ODF_arg->lastSegment = 1;
        memcpy(ODF_arg->data, odFileData->fileData, ODF_arg->dataLength);
    }
    else { 
        /* store parameters */
        if(ODF_arg->firstSegment) {
            /* 1st segment */
            odFileData->fileSize = ODF_arg->dataLength;
            ODF_arg->firstSegment = 0;

            if(ODF_arg->dataLength > CO_COMMAND_SDO_BUFFER_SIZE) {
                ret = CO_SDO_AB_OUT_OF_MEM; 
                return ret;
            }
        }

        ODF_arg->lastSegment = 1;
        memcpy(odFileData->fileData, ODF_arg->data, ODF_arg->dataLength);
    }

    return ret;
}


CO_ReturnError_t CO_OD_file_transfer_init(CO_OD_file_data_t *odFileData) {
    odFileData->fileData = (uint8_t *)malloc(CO_COMMAND_SDO_BUFFER_SIZE);
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
