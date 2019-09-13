/**
 * Command interface for CANdaemon.
 */


#ifndef CD_DBUS_H
#define CD_DBUS_H


#include <stdint.h>


/* Functions from external */
void CO_errExit(char* msg);
void CO_error(const uint32_t info);


/**
 * Initialize thread and create socket for dbus thread.
 *
 * @return 0 on success.
 */
int dbus_init(void);


/**
 * Terminate thread and closes dbus thread.
 *
 * @return 0 on success.
 */
int dbus_clear(void);


#endif
