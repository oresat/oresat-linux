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
#include "error_assert_handlers.h"
#include "CO_driver.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <systemd/sd-bus.h>
#include <pthread.h>
#include <signal.h>


#define INTERFACE_NAME  "org.OreSat.StarTracker"
#define BUS_NAME        INTERFACE_NAME
#define OBJECT_PATH     "/org/OreSat/StarTracker"
#define SIGNAL_THREAD_STACK_SIZE FILE_TRANSFER_MAX_SIZE*3


/* Static Variables */
static pthread_t        signal_thread_id;
static pthread_attr_t   signal_thread_attr;
static volatile int     endProgram = 0;
static sd_bus           *bus = NULL;


/* Static Functions */
static void* signal_thread(void *arg);
static int file_transfer_signal_cb(sd_bus_message *, void *, sd_bus_error *);
static int data_signal_cb(sd_bus_message *, void *, sd_bus_error *);


/******************************************************************************/
void app_programStart(void){
    int r;
    endProgram = 0;

    /* Connect to the system bus */
    r = sd_bus_open_system(&bus);
    dbusErrorExit(r, "Failed to connect to system bus.");

    pthread_attr_setstacksize(&signal_thread_attr, SIGNAL_THREAD_STACK_SIZE);
    r = pthread_create(&signal_thread_id, &signal_thread_attr, signal_thread, NULL);
    dbusError(-r, "signal thread creation failed"); // this r need to be negative

    return;
}


/******************************************************************************/
void app_communicationReset(void){

}


/******************************************************************************/
void app_programEnd(void){
    endProgram = 1;

    /* Wait for threads to finish. */
    int r = pthread_join(signal_thread_id, NULL);
    dbusError(-r, "signal thread join failed"); // this r need to be negative

    sd_bus_unref(bus);

    return;
}


/******************************************************************************/
void app_programAsync(uint16_t timer1msDiff){

}


/******************************************************************************/
void app_program1ms(void){

}


/****************************************************************************/
static void* signal_thread(void *arg) {
    sd_bus_slot *slot = NULL;
    int r;

    /* Catch signals SIGINT and SIGTERM */
    if(signal(SIGINT, NULL) == SIG_ERR)
        dbusErrorExit(0, "Program init - SIGINIT handler creation failed");
    if(signal(SIGTERM, NULL) == SIG_ERR)
        dbusErrorExit(0, "Program init - SIGTERM handler creation failed");

    /* add signal matches here */
    r = sd_bus_add_match(bus,
                         &slot,
                        "type='signal', path='"OBJECT_PATH"', interface='"INTERFACE_NAME"', member='OrientationSignal'",
                         data_signal_cb, 
                         NULL);
    dbusErrorExit(r, "Add match error for data signal.");

    r = sd_bus_add_match(bus,
                         &slot,
                        "type='signal', path='"OBJECT_PATH"', interface='"INTERFACE_NAME"', member='NewImageSignal'",
                         file_transfer_signal_cb, 
                         NULL);
    dbusErrorExit(r, "Add match error for file transfer signal.");
    
    /* wait for interupt and loop */
    while(endProgram == 0) {
        r = sd_bus_process(bus, NULL);
        dbusError(r, "Bus process failed.");
        if(r > 0)
            continue;

        r = sd_bus_wait(bus, (uint64_t)-1);
        dbusError(r, "Bus wait failed.");
    }

    sd_bus_slot_unref(slot);
    return NULL;
}


/* callback for handling file transfer from the Star Tracker process */
static int file_transfer_signal_cb(sd_bus_message *m, void *user_data, sd_bus_error *ret_error) {
    int r;
    char *filepath = NULL;

    r = sd_bus_message_read(m, "s", &filepath);
    dbusError(r, "Failed to parse file transfer signal.");

    if (r < 0)
        return -1;

    APP_ODF_3002(filepath);

    return 0;
}


/* callback for reading the data signal from the Star Tracker process */
static int data_signal_cb(sd_bus_message *m, void *user_data, sd_bus_error *ret_error) {
    int r;
    int16_t rotationY = 0; // roatation about the y axis
    int16_t rotationZ = 0; // rotation about the z axis
    int16_t orientation = 0; // rotation about the camera axis

    r = sd_bus_message_read(m, "nnn", &rotationY, &rotationZ, &orientation);
    dbusError(r, "Failed to parse data signal.");

    if (r < 0)
        return -1;

    app_writeOD(0x3101, 1, &rotationY, sizeof(rotationY));
    app_writeOD(0x3101, 2, &rotationZ, sizeof(rotationZ));
    app_writeOD(0x3101, 3, &orientation, sizeof(orientation));

    return 0;
}

