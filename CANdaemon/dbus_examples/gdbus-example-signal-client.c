#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <gio/gio.h>
#include <gio/gunixfdlist.h>

#define INTERFACE_NAME "org.example.project.oresat" // must be the same in XML
#define BUS_NAME INTERFACE_NAME                     // same as inface name, for now
#define OBJECT_PATH "/org/example/project/oresat"

 static void
read_data_signal (GDBusConnection  *connection,
                  const gchar      *sender_name,
                  const gchar      *object_path,
                  const gchar      *interface_name,
                  const gchar      *signal_name,
                  GVariant         *parameters,
                  gpointer         user_data)
{
    gchar *test_string = NULL;
    gdouble test_double = 0.0;
    gint test_int = 0;
    GMainLoop *loop = g_main_loop_new (NULL, FALSE);

    g_variant_get (parameters,
                   "(&sdi)",
                   &test_string,
                   &test_double,
                   &test_int);

    g_print ("%s %f %d\n", 
                test_string, 
                test_double,
                test_int);

    g_main_loop_run (loop);
    g_free (test_string);

}

static void
read_file_signal (GDBusConnection  *connection,
                  const gchar      *sender_name,
                  const gchar      *object_path,
                  const gchar      *interface_name,
                  const gchar      *signal_name,
                  GVariant         *parameters,
                  gpointer         user_data)
{
    gchar *test_string = NULL;
    GMainLoop *loop = g_main_loop_new (NULL, FALSE);

    g_variant_get (parameters,
                   "(&s)",
                   &test_string);

    g_print ("%s\n", 
                test_string);

    g_main_loop_run (loop);
    g_free (test_string);
}


static void
on_name_appeared (GDBusConnection *connection,
                  const gchar     *name,
                  const gchar     *name_owner,
                  gpointer         user_data)
{
    guint s1;
    guint s2;
    g_assert (connection != NULL);
    g_assert (!g_dbus_connection_is_closed (connection));

    GMainLoop *loop = g_main_loop_new (NULL, FALSE);

    // subscribe to data signal
    s1 = g_dbus_connection_signal_subscribe (connection,
                                           INTERFACE_NAME,
                                           INTERFACE_NAME,
                                           "data_signal",
                                           OBJECT_PATH,
                                           NULL,
                                           G_DBUS_SIGNAL_FLAGS_NONE,
                                           read_data_signal,
                                           user_data,
                                           NULL);

    // subscribe to file signal
    s2 = g_dbus_connection_signal_subscribe (connection,
                                           INTERFACE_NAME,
                                           INTERFACE_NAME,
                                           "file_signal",
                                           OBJECT_PATH,
                                           NULL,
                                           G_DBUS_SIGNAL_FLAGS_NONE,
                                           read_file_signal,
                                           user_data,
                                           NULL);

    g_main_loop_run (loop);

    g_dbus_connection_signal_unsubscribe (connection, s1);
    g_dbus_connection_signal_unsubscribe (connection, s2);
}


int
main (int argc, char *argv[]) 
{
    GMainLoop *loop = g_main_loop_new (NULL, FALSE);
    
    guint watcher_id = g_bus_watch_name (G_BUS_TYPE_SESSION,
                                    INTERFACE_NAME,
                                    G_BUS_NAME_WATCHER_FLAGS_NONE,
                                    on_name_appeared,
                                    NULL,
                                    NULL,
                                    NULL);

    g_main_loop_run (loop);

    g_bus_unwatch_name (watcher_id);
    return 0;
}

