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
 * Initialize thread and create socket for dbus interface.
 *
 * @return 0 on success.
 */
int CD_dbus_init(void);


/**
 * Terminate thread and closes dbus interface
 *
 * @return 0 on success.
 */
int CD_command_clear(void);


/**
 * Read data signal
 */
/*static void
read_data_signal (GDBusConnection  *connection,
                  const gchar     *name,
                  const gchar     *name_owner,
                  gpointer         user_data);
                  */


/**
 * Read file signal
 */
/*
static void
read_file_signal (GDBusConnection  *connection,
                  const gchar     *name,
                  const gchar     *name_owner,
                  gpointer         user_data);

*/
#endif
