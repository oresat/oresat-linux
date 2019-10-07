
#ifndef CANDAEMON_H
#define CANDAEMON_H


#include <stdint.h>


/**
 * Function is called on program startup.
 */
void CD_programStart(void);


/**
 * Function is called after CANopen communication reset.
 */
void CD_communicationReset(void);


/**
 * Function is called just before program ends.
 */
void CD_programEnd(void);


/**
 * Function is called cyclically from main.
 *
 * @param timer1msDiff Time difference since last call
 */
void CD_programAsync(uint16_t timer1msDiff);


/**
 * Function is called cyclically from realtime thread at constant intervals.
 *
 * Code inside this function must be executed fast. Take care on race conditions.
 */
void CD_program1ms(void);


#endif
