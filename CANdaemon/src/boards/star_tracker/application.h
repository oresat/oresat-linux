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


#endif
