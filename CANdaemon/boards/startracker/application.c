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
#include "error_assert_handlers.h"
#include "CANopen.h"
#include "CO_driver.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <systemd/sd-bus.h>
#include <signal.h>


#define INTERFACE_NAME  "org.OreSat.StarTracker"
#define BUS_NAME        INTERFACE_NAME
#define OBJECT_PATH     "/org/OreSat/StarTracker"
#define SIGNAL_THREAD_STACK_SIZE FILE_TRANSFER_MAX_SIZE*3


/* Static Variables */
static sd_bus           *bus = NULL;


/* Static Functions */


/******************************************************************************/
void app_programStart(void){
    int r;

    /* Connect to the system bus */
    r = sd_bus_open_system(&bus);
    dbusErrorExit(r, "Failed to connect to system bus.");

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
    sd_bus_error err = SD_BUS_ERROR_NULL;
    sd_bus_message *mess = NULL;
    double declination, right_ascension, orientation = 0.0;
    int r;

    r = sd_bus_get_property(bus,BUS_NAME, OBJECT_PATH, INTERFACE_NAME, "DEC", &err, &mess, "d");
    if (r >= 0) {
        /* decode dbus property */
        r = sd_bus_message_read(mess, "d", &declination);
    }

    /* free message */
    sd_bus_error_free(&err);
    err = SD_BUS_ERROR_NULL;
    sd_bus_message_unref(mess);
    mess = NULL;

    r = sd_bus_get_property(bus, BUS_NAME, OBJECT_PATH, INTERFACE_NAME, "RA",  &err, &mess, "d");
    if (r >= 0) {
        /* decode dbus property */
        r = sd_bus_message_read(mess, "d", &right_ascension);
    }

    /* free message */
    sd_bus_error_free(&err);
    err = SD_BUS_ERROR_NULL;
    sd_bus_message_unref(mess);
    mess = NULL;

    r = sd_bus_get_property(bus, BUS_NAME, OBJECT_PATH, INTERFACE_NAME, "ORI",  &err, &mess, "d");
    if (r >= 0) {
        /* decode dbus property */
            r = sd_bus_message_read(mess, "d", &orientation);
    }

    /* free message */
    sd_bus_error_free(&err);
    err = SD_BUS_ERROR_NULL;
    sd_bus_message_unref(mess);
    mess = NULL;

    app_writeOD(0x3101, 1, &declination, sizeof(declination));
    app_writeOD(0x3101, 2, &right_ascension, sizeof(right_ascension));
    app_writeOD(0x3101, 3, &orientation, sizeof(orientation));
}


