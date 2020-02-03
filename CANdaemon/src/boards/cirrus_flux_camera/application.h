#ifndef APPLICATION_H
#define APPLICATION_H


#include "CANopen.h"
#include "CO_driver.h"

/**
 * Function is called on program startup.
 */
int app_dbus_setup(void);


/**
 * Function is called just before program ends.
 */
int app_dbus_end(void);


/**
 * Callbacks for getting the latest image from the test camera process and adding to the send bufffer.
 */
CO_SDO_abortCode_t CFC_ODF(CO_ODF_arg_t *ODF_arg);


#endif
