#ifndef SYSTEMD_ODF_H
#define SYSTEMD_ODF_H


#include "CANopen.h"
#include "CO_driver.h"


int systemd_ODF_setup(void);


CO_SDO_abortCode_t systemd_ODF(CO_ODF_arg_t *ODF_arg);


#endif
