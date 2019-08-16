/**
 * Command interface for CANdaemon.
 */


#ifndef CD_DBUS_H
#define CD_DBUS_H


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
 * Initialize thread and create socket for dbus interface.
 */
static void* signal_thread(void* arg) {


/**
 * Terminate thread and closes dbus interface
 *
 * @return 0 on success.
 */
int CD_command_clear(void);


/**
 * Terminate thread and closes dbus interface
 */
static void
test_connection_callback (GDBusConnection  *connection,
                          const gchar      *sender_name,
                          const gchar      *object_path,
                          const gchar      *interface_name,
                          const gchar      *signal_name,
                          GVariant         *parameters,
                          gpointer         user_data);


/**
 * Terminate thread and closes dbus interface
 */
static void
read_data_signal (GDBusConnection  *connection,
                  const gchar     *name,
                  const gchar     *name_owner,
                  gpointer         user_data);


/**
 * Terminate thread and closes dbus interface
 */
static void
read_file_signal (GDBusConnection  *connection,
                  const gchar     *name,
                  const gchar     *name_owner,
                  gpointer         user_data);


#endif
