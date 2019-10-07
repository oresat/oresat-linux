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
#ifdef GPS_INTERFACE
#include "GPS_interface.h"
#endif
#ifdef ST_INTERFACE
#include "ST_interface.h"
#endif
#ifdef UPDATER_INTERFACE
#include "Updaer_interface.h"
#endif
#include <stdint.h>


/******************************************************************************/
void app_programStart(void){
    app_ODF_configure();

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


