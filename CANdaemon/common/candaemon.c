
#include "candaemon.h"
#include "application.h"
#include "app_OD_functions.h"
#include <stdint.h>


/******************************************************************************/
void CD_programStart(void){
    app_ODF_configure();
    app_programStart();

    return;
}


/******************************************************************************/
void CD_communicationReset(void){
    app_communicationReset();

}


/******************************************************************************/
void CD_programEnd(void){
    app_programEnd();

    return;
}


/******************************************************************************/
void CD_programAsync(uint16_t timer1msDiff){
    app_programAsync(timer1msDiff);

    return;
}


/******************************************************************************/
void CD_program1ms(void){
    app_program1ms();

    return;
}


