
#include "candaemon.h"
#include "app_OD_functions.h"
#include "systemd_ODF.h"
#include "application.h"
#include "updater.h"
#include <stdint.h>


/******************************************************************************/
void CD_programStart(void){
    app_ODF_configure();
    systemd_ODF_configure();

    app_programStart();

    updater_programStart();

    return;
}


/******************************************************************************/
void CD_communicationReset(void){
    app_communicationReset();
    updater_communicationReset();

    return;
}


/******************************************************************************/
void CD_programEnd(void){
    app_programEnd();
    updater_communicationReset();

    return;
}


/******************************************************************************/
void CD_programAsync(uint16_t timer1msDiff){
    app_programAsync(timer1msDiff);
    updater_programAsync(timer1msDiff);

    return;
}


/******************************************************************************/
void CD_program1ms(void){
    app_program1ms();
    updater_program1ms();

    return;
}


