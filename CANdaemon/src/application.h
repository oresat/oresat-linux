/**
 * Application interface for CANopenSocket.
 *
 * @file        application.h
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


#ifndef CO_APPLICATION_H
#define CO_APPLICATION_H


#include "CO_driver.h"
#include "CO_SDO.h"


/**
 * Object Dictionary storage object.
 *
 * Object is used with CANopen OD objects that are for file transfer.
 */
typedef struct {
    uint8_t    *fileData;       /**< From CO_OD_file_transfer_init() */
    uint32_t    fileSize;       /**< From CO_OD_file_transfer_init() */
} CO_OD_file_data_t;


/**
 * Function is called on program startup.
 */
void app_programStart(void);


/**
 * Function is called after CANopen communication reset.
 */
void app_communicationReset(void);


/**
 * Function is called just before program ends.
 */
void app_programEnd(void);


/**
 * Function is called cyclically from main.
 *
 * @param timer1msDiff Time difference since last call
 */
void app_programAsync(uint16_t timer1msDiff);


/**
 * Function is called cyclically from realtime thread at constant intervals.
 *
 * Code inside this function must be executed fast. Take care on race conditions.
 */
void app_program1ms(void);


/**
 * Callbacks for using inside @ref CO_OD_configure() function for OD DOMAIN objects.
 */
CO_SDO_abortCode_t file_transfer(CO_ODF_arg_t *ODF_arg);


/**
 * Constructor for data file struct.
 */
CO_ReturnError_t CO_OD_file_transfer_init(CO_OD_file_data_t *odFileData);


/**
 * Deconstructor for data file struct.
 */
void CO_OD_file_transfer_close(CO_OD_file_data_t *odFileData);


#endif
