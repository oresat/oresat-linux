#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <gio/gio.h>
#include <gio/gunixfdlist.h>

#define DBUS_INTERFACE "org.example.project_1.server_1"
#define DBUS_PATH "/org/example/project_1/server_1"

static void
test_connection_callback (GDBusConnection  *connection,
                          const gchar      *sender_name,
                          const gchar      *object_path,
                          const gchar      *interface_name,
                          const gchar      *signal_name,
                          GVariant         *parameters,
                          gpointer         user_data)
{
    gchar *test_string = NULL;
    gdouble test_double = 0.0;
    GMainLoop *loop = g_main_loop_new (NULL, FALSE);

    g_variant_get (parameters,
                   "(&sd)",
                   &test_string,
                   test_double);

    g_print ("%s %f\n", 
                test_string, 
                test_double);

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
    g_assert (connection != NULL);
    g_assert (!g_dbus_connection_is_closed (connection));

    GMainLoop *loop = g_main_loop_new (NULL, FALSE);
    s1 = g_dbus_connection_signal_subscribe (connection,
                                           DBUS_INTERFACE,
                                           DBUS_INTERFACE,
                                           "app_1_signal",
                                           DBUS_PATH,
                                           NULL,
                                           G_DBUS_SIGNAL_FLAGS_NONE,
                                           test_connection_callback,
                                           user_data,
                                           NULL);

    g_main_loop_run (loop);

    g_dbus_connection_signal_unsubscribe (connection, s1);
}


static void
on_name_vanished (GDBusConnection *connection,
                  const gchar     *name,
                  gpointer         user_data)
{
  g_printerr ("Failed to get name owner for %s\n"
              "Is server running?\n",
              name);
  //exit (1);
}


int
main (int argc, char *argv[]) 
{
    GMainLoop *loop = g_main_loop_new (NULL, FALSE);
    
    guint watcher_id = g_bus_watch_name (G_BUS_TYPE_SESSION,
                                    DBUS_INTERFACE,
                                    G_BUS_NAME_WATCHER_FLAGS_NONE,
                                    on_name_appeared,
                                    on_name_vanished,
                                    NULL,
                                    NULL);

    g_main_loop_run (loop);

    g_bus_unwatch_name (watcher_id);
    return 0;
}

