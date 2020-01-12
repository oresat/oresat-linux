/**
 *
 */


#ifndef UPDATER_H
#define UPDATER_H


#include "CANopen.h"
#include "CO_driver.h"
#include <stdint.h>


/**
 * Function is called on program startup.
 */
void updater_programStart(void);


/**
 * Function is called after CANopen communication reset.
 */
void updater_communicationReset(void);


/**
 * Function is called just before program ends.
 */
void updater_programEnd(void);


/**
 * Function is called cyclically from main.
 *
 * @param timer1msDiff Time difference since last call
 */
void updater_programAsync(uint16_t timer1msDiff);


/**
 * Function is called cyclically from realtime thread at constant intervals.
 *
 * Code inside this function must be executed fast. Take care on race conditions.
 */
void updater_program1ms(void);


/**
 * Callbacks for getting the latest image from the test camera process and adding to the send bufffer.
 */
CO_SDO_abortCode_t CO_ODF_3004(CO_ODF_arg_t *ODF_arg);


#endif
