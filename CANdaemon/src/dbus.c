/*
 * DBUS
 */


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <endian.h>
#include <errno.h>
#include <pthread.h>
#include <sys/un.h>
#include <sys/types.h>
#include <time.h>
#include <gio/gio.h>
#include <gio/gunixfdlist.h>

#include "config.h"
#include "CANopen.h"
#include "dbus.h"
#include "CO_comm_helpers.h"
#include "CO_master.h"
#include "dbus_helpers.h"


/* Maximum size of Object Dictionary variable transmitted via SDO. */
#define SDO_BUFFER_SIZE     100000 // TODO move to config.h

#define STRING_BUFFER_SIZE  (SDO_BUFFER_SIZE * 4 + 100)
#define LISTEN_BACKLOG      50


/* Variables */
static void*                signal_thread(void* arg);
static pthread_t            signal_thread_id;
static void                 command_process(char * );
static uint16_t             SDOtimeoutTime = 500; /* Timeout time for SDO transfer in milliseconds, if no response */
static uint8_t              blockTransferEnable = 0; /* SDO block transfer enabled? */
static volatile int         endProgram = 0;
static void read_data_signal (GDBusConnection *connection, const gchar *sender_name,
                              const gchar *object_path, const gchar *interface_name,
                              const gchar *signal_name, GVariant *parameters,
                              gpointer user_data);
static void read_file_signal (GDBusConnection  *connection,
                  const gchar      *sender_name,
                  const gchar      *object_path,
                  const gchar      *interface_name,
                  const gchar      *signal_name,
                  GVariant         *parameters,
                  gpointer         user_data);
static void on_name_appeared (GDBusConnection *connection,
                  const gchar     *name,
                  const gchar     *name_owner,
                  gpointer         user_data);


/******************************************************************************/
int CD_command_init(void) {

    /* Create thread */
    endProgram = 0;
    if(pthread_create(&signal_thread_id, NULL, signal_thread, NULL) != 0) {
        CO_errExit("CD_command_init - thread creation failed");
    }

    return 0;
}


/******************************************************************************/
int CD_command_clear(void) {

    // TODO code here

    endProgram = 1;

    /* Wait for thread to finish. */
    if(pthread_join(signal_thread_id, NULL) != 0) {
        return -1;
    }

    return 0;
}


/******************************************************************************/
static void* signal_thread() {
    GMainLoop *loop = g_main_loop_new (NULL, FALSE);
    
    guint watcher_id = g_bus_watch_name (G_BUS_TYPE_SESSION,
                                    DBUS_INTERFACE,
                                    G_BUS_NAME_WATCHER_FLAGS_NONE,
                                    on_name_appeared,
                                    NULL,
                                    NULL,
                                    NULL);

    g_main_loop_run (loop);

    g_bus_unwatch_name (watcher_id);

    // TODO add loop escape logic
    return NULL;
}


/******************************************************************************/
static void read_data_signal (GDBusConnection  *connection,
                              const gchar      *sender_name,
                              const gchar      *object_path,
                              const gchar      *interface_name,
                              const gchar      *signal_name,
                              GVariant         *parameters,
                              gpointer         user_data) {

    gchar *test_string = NULL;
    gdouble test_double = 0.0;
    GMainLoop *loop = g_main_loop_new (NULL, FALSE);

    g_variant_get (parameters,
                   "(&sd)",
                   &test_string,
                   &test_double);

    g_print ("%s %f\n", 
                test_string, 
                test_double);

    // TODO make PDO
    // TODO call PDO sender

    g_main_loop_run (loop);
    g_free (test_string);
}


/******************************************************************************/
static void read_file_signal (GDBusConnection  *connection,
                              const gchar      *sender_name,
                              const gchar      *object_path,
                              const gchar      *interface_name,
                              const gchar      *signal_name,
                              GVariant         *parameters,
                              gpointer         user_data) {

    gchar *file_path = NULL;
    GMainLoop *loop = g_main_loop_new (NULL, FALSE);

    g_variant_get(parameters, "(&s)", &file_path);

    g_print("Filepath: %s\n", file_path); // TODO remove

    send_file(file_path, 
              0x1010, // idx for filename
              0,      // subidx for file name
              0x1017, // idx for file data, 
              0);     // subidx for file data

    g_main_loop_run (loop);
    g_free (file_path);
}


/******************************************************************************/
static void on_name_appeared (GDBusConnection *connection,
                              const gchar     *name,
                              const gchar     *name_owner,
                              gpointer         user_data) {
    //guint s1;
    guint s2;
    g_assert (connection != NULL);
    g_assert (!g_dbus_connection_is_closed (connection));

    GMainLoop *loop = g_main_loop_new (NULL, FALSE);

    /*
    // subscribe to data signal
    s1 = g_dbus_connection_signal_subscribe (connection,
                                           DBUS_INTERFACE,
                                           DBUS_INTERFACE,
                                           "data_signal",
                                           DBUS_PATH,
                                           NULL,
                                           G_DBUS_SIGNAL_FLAGS_NONE,
                                           read_data_signal,
                                           user_data,
                                           NULL);
    */

    // subscribe to file signal
    s2 = g_dbus_connection_signal_subscribe (connection,
                                           DBUS_INTERFACE,
                                           DBUS_INTERFACE,
                                           "file_signal",
                                           DBUS_PATH,
                                           NULL,
                                           G_DBUS_SIGNAL_FLAGS_NONE,
                                           read_file_signal,
                                           user_data,
                                           NULL);

    g_main_loop_run (loop);

    //g_dbus_connection_signal_unsubscribe (connection, s1);
    g_dbus_connection_signal_unsubscribe (connection, s2);
}

