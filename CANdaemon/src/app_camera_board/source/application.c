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


#include "application.h"
#include "app_OD_functions.h"
#include "app_OD_helpers.h"
#include "CANopen.h"
#include "CO_driver.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <systemd/sd-bus.h>


#define INTERFACE_NAME  "org.OreSat.CameraBoard"
#define BUS_NAME        INTERFACE_NAME
#define OBJECT_PATH     "/org/OreSat/CameraBoard"
#define SIGNAL_THREAD_STACK_SIZE FILE_TRANSFER_MAX_SIZE*3


/* Static Variables */
static volatile int     endProgram = 0;
static sd_bus          *bus = NULL;


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
void app_programStart(void){
    int r;

    /* Connect to the bus */
    r = sd_bus_open_system(&bus);
    dbusError(r, "Failed to connect to system bus.");

    CO_OD_configure(CO->SDO[0], 0x3100, CB_ODF_3100, NULL, 0, 0U);

    return;
}


/******************************************************************************/
void app_communicationReset(void){

}


/******************************************************************************/
void app_programEnd(void){
    sd_bus_unref(bus);

    return;
}


/******************************************************************************/
void app_programAsync(uint16_t timer1msDiff){

}


/******************************************************************************/
void app_program1ms(void){

}


/******************************************************************************/


CO_SDO_abortCode_t CB_ODF_3100(CO_ODF_arg_t *ODF_arg) {
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
    r = sd_bus_call_method(bus,
                           BUS_NAME,
                           OBJECT_PATH,
                           INTERFACE_NAME,
                           "LatestImage",
                           &err,
                           &m,
                           NULL);
    dbusError(r, "Failed to issue method call.");

    /* Parse the response message */
    r = sd_bus_message_read(m, "s", &file_path);
    dbusError(r, "Failed to parse response message.");

    if(file_path != NULL)
        APP_ODF_3002(file_path);

    sd_bus_error_free(&err);
    sd_bus_message_unref(m);

    ODF_arg->lastSegment = true;

    return ret;
}

