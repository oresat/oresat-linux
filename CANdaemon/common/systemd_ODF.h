/*
 *
 */


#ifndef SYSTEMD_ODF_H
#define SYSTEMD_ODF_H


#include "CANopen.h"
#include "CO_driver.h"

void systemd_ODF_configure(void);
void end_systemd_ODF(void);

CO_SDO_abortCode_t SYSTEMD_ODF(CO_ODF_arg_t *ODF_arg);

#endif
