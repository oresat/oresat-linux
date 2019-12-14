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
#include <pthread.h>
#include <signal.h>


#define INTERFACE_NAME  "org.OreSat.GPS"
#define BUS_NAME        INTERFACE_NAME
#define OBJECT_PATH     "/org/OreSat/GPS"
#define SIGNAL_THREAD_STACK_SIZE FILE_TRANSFER_MAX_SIZE*3


/* Static Variables */
static pthread_t        signal_thread_id;
static pthread_attr_t   signal_thread_attr;
static volatile int     endProgram = 0;
static sd_bus           *bus = NULL;


/* Static Functions */
static void* signal_thread(void *arg);
static int status_signal_cb(sd_bus_message *, void *, sd_bus_error *);
static int data_signal_cb(sd_bus_message *, void *, sd_bus_error *);
static void updateState(void);


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
    /*
    r = sd_bus_add_match(bus,
                         &slot,
                        "type='signal', path='"OBJECT_PATH"', interface='"INTERFACE_NAME"', member='StatusSignal'", 
                         status_signal_cb, 
                         NULL);
    dbusErrorExit(r, "Add match error for status signal.");
    */

    r = sd_bus_add_match(bus,
                         &slot,
                        "type='signal', path='"OBJECT_PATH"', interface='"INTERFACE_NAME"', member='StateVectorSignal'",
                         data_signal_cb, 
                         NULL);
    dbusErrorExit(r, "Add match error for state vector signal.");

    
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


/* callback for reading the status signal form the GPS */
static int status_signal_cb(sd_bus_message *m, void *user_data, sd_bus_error *ret_error) {
    int r;
    int32_t state;

    r = sd_bus_message_read(m, "i", &state);
    dbusError(r, "Failed to parse status signal.");
    if (r < 0)
        return -1;

    app_writeOD(0x3001, 1, &state, sizeof(state));

    return 0;
}


/* callback for reading the data signal from the GPS */
static int data_signal_cb(sd_bus_message *m, void *user_data, sd_bus_error *ret_error) {
    int r;
    int16_t posX = 0;
    int16_t posY = 0;
    int16_t posZ = 0;
    /*
    int16_t velX = 0;
    int16_t velY = 0;
    int16_t velZ = 0;
    int16_t accX = 0;
    int16_t accY = 0;
    int16_t accZ = 0;*/

    r = sd_bus_message_read(m, "nnn", &posX, &posY, &posZ); //, &velX, &velY, &velZ, &accX, &accY, &accZ);
    dbusError(r, "Failed to parse data signal.");

    if (r < 0)
        return -1;

    app_writeOD(0x3003, 1, &posX, sizeof(posX));
    app_writeOD(0x3003, 2, &posY, sizeof(posY));
    app_writeOD(0x3003, 3, &posZ, sizeof(posZ));/*
    app_writeOD(0x3003, 4, &velX, sizeof(velX));
    app_writeOD(0x3003, 5, &velY, sizeof(velY));
    app_writeOD(0x3003, 6, &velZ, sizeof(velZ));
    app_writeOD(0x3003, 7, &accX, sizeof(accX));
    app_writeOD(0x3003, 8, &accY, sizeof(accY));
    app_writeOD(0x3003, 9, &accZ, sizeof(accZ));*/

    return 0;
}


/****************************************************************************/
/* methods for main dbus interface thread to call */

/*
/ * Handle new state change with dbus method call to GPS process * /
static void updateState(void) {
    int r;
    sd_bus_error error = SD_BUS_ERROR_NULL;
    sd_bus_message *m = NULL;
    int32_t current_state, new_state, return_int;

    app_readOD(0x3000, 1, &new_state, sizeof(new_state));
    app_readOD(0x3001, 1, &current_state, sizeof(current_state));

    if(new_state == current_state)
        return; / * no need to change * /

    / * Issue the method call and store the response message in m * /
    r = sd_bus_call_method(bus,
                           BUS_NAME,
                           OBJECT_PATH,
                           INTERFACE_NAME,
                           "ChangeState",
                           &error,
                           &m,
                           "i",
                           new_state);
    dbusError(r, "Failed to issue method call.");

    / * Parse the response message * /
    r = sd_bus_message_read(m, "i", &return_int);
    dbusError(r, "Failed to parse response message.");

    sd_bus_error_free(&error);
    return;
}
*/

