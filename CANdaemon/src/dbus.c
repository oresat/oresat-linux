/*
 * DBUS
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <gio/gio.h>
#include <gio/gunixfdlist.h>


#include "configs.h"
#include "CANopen.h"
#include "dbus.h"
#include "CO_comm_helpers.h"
#include "CO_master.h"
#include "dbus_helpers.h"


/* DBUS setting */
#define DBUS_INTERFACE  "org.example.project.oresat"    // must be the same in XML
#define DBUS_NAME       DBUS_INTERFACE                  // same as inface name, for now
#define DBUS_PATH       "/org/example/project/oresat"   

/* Variables */
static pthread_t            signal_thread_id;
static volatile int         endProgram = 0;

/* static functions */
static void*                signal_thread(void *);
/*static void                 read_data_signal (GDBusConnection *connection, 
                                              const gchar *sender_name,
                                              const gchar *object_path, 
                                              const gchar *interface_name,
                                              const gchar *signal_name, 
                                              GVariant *parameters,
                                              gpointer user_data);*/
static void                 read_file_signal (GDBusConnection  *connection,
                                              const gchar      *sender_name,
                                              const gchar      *object_path,
                                              const gchar      *interface_name,
                                              const gchar      *signal_name,
                                              GVariant         *parameters,
                                              gpointer         user_data);
static void                 on_name_appeared (GDBusConnection *connection,
                                              const gchar     *name,
                                              const gchar     *name_owner,
                                              gpointer         user_data);


int dbus_init(void) {

    if(CO == NULL || CO->SDOclient == NULL){
        CO_errExit("dbus_init - Wrong arguments");
    }

    /* Create thread */
    endProgram = 0;
    if(pthread_create(&signal_thread_id, NULL, signal_thread, NULL) != 0) {
        CO_errExit("dbus_init - thread creation failed");
    }

    return 0;
}


int dbus_clear(void) {

    // TODO code here

    endProgram = 1;

    /* Wait for thread to finish. */
    if(pthread_join(signal_thread_id, NULL) != 0) {
        return -1;
    }

    return 0;
}


static void* signal_thread(void *arg) {
    GMainLoop *loop = g_main_loop_new (NULL, FALSE);
    
    guint watcher_id = g_bus_watch_name (G_BUS_TYPE_SESSION,
                                        DBUS_NAME,
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

/*
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
*/


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

    send_file(file_path, 
              0x1201, // idx
              1,      // subidx for file name
              2);     // subidx for file data

    g_main_loop_run (loop);
    g_free (file_path);
}


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
                                           DBUS_NAME,
                                           DBUS_NAME,
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
                                           DBUS_NAME,
                                           DBUS_NAME,
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

