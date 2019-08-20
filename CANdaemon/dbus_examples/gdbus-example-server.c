#include <stdlib.h>
#include <unistd.h>
#include <gio/gunixfdlist.h>
#include <gio/gio.h>

#define INTERFACE_NAME "org.example.project.oresat" // must be the same in XML
#define BUS_NAME INTERFACE_NAME                     // same as inface name, for now
#define OBJECT_PATH "/org/example/project/oresat"

/* ---------------------------------------------------------------------------------------------------- */

static GDBusNodeInfo *introspection_data = NULL;

/* Introspection data for the service we are exporting */
static const gchar introspection_xml[] =
    "<node>"
    "   <interface name='org.example.project.oresat'>"
    "       <method name='Command'>"
    "           <arg type='s' name='command' direction='in'/>"
    "           <arg type='s' name='response' direction='out'/>"
    "       </method>"
    "       <method name='Quit'/>"
    "       <signal name='data_signal'>"
    "           <arg type='s'/>"
    "           <arg type='d'/>"
    "           <arg type='i'/>"
    "       </signal>"
    "       <signal name='file_signal'>"
    "           <arg type='s'/>"
    "       </signal>"
    "   </interface>"
    "</node>";

/* ---------------------------------------------------------------------------------------------------- */

static void
handle_method_call (GDBusConnection       *connection,
                    const gchar           *sender,
                    const gchar           *object_path,
                    const gchar           *interface_name,
                    const gchar           *method_name,
                    GVariant              *parameters,
                    GDBusMethodInvocation *invocation,
                    gpointer               user_data)
{
  if (g_strcmp0 (method_name, "Command") == 0)
    {
        if (g_dbus_connection_get_capabilities (connection) & G_DBUS_CAPABILITY_FLAGS_UNIX_FD_PASSING)
        {
            GDBusMessage *reply;
            GUnixFDList *fd_list;
            gchar * test_string = NULL;
            GError *error = NULL;

            fd_list = g_unix_fd_list_new ();
            g_unix_fd_list_append (fd_list, STDOUT_FILENO, &error);
            g_assert_no_error (error);

            g_variant_get (parameters, "(&s)", &test_string); 
            g_print("%s\n", test_string);

            // TODO no return value
            //g_dbus_method_invocation_return_value (invocation,
            //                                        g_variant_new ("(s)", "done"));

            reply = g_dbus_message_new_method_reply (g_dbus_method_invocation_get_message (invocation));
            g_dbus_message_set_unix_fd_list (reply, fd_list);

            error = NULL;
            g_dbus_connection_send_message (connection,
                                          reply,
                                          G_DBUS_SEND_MESSAGE_FLAGS_NONE,
                                          NULL, /* out_serial */
                                          &error);
            g_assert_no_error (error);

            g_object_unref (invocation);
            //g_object_unref (fd_list);
            //g_object_unref (reply);
        }
    }
    else if (g_strcmp0 (method_name, "Quit") == 0)
    {
      gchar *response;
      response = g_strdup_printf ("Quit");
      g_dbus_method_invocation_return_value (invocation,
                                             g_variant_new ("(s)", response));
      g_free (response);
      exit(0); // TODO change this
    }
    else {
      g_print("%s not recognized", method_name);
      g_dbus_method_invocation_return_dbus_error (invocation,
                                                  "org.gtk.GDBus.NotOnUnix",
                                                  "Your OS does not support file descriptor passing");
    }
}

/* for now */
static const GDBusInterfaceVTable interface_vtable =
{
  handle_method_call,
  //add more methods function names, as needed, here
};


/* ---------------------------------------------------------------------------------------------------- */

static void
on_bus_acquired (GDBusConnection *connection,
                 const gchar     *name,
                 gpointer         user_data)
{
  guint registration_id;

  registration_id = g_dbus_connection_register_object (connection,
                                                       OBJECT_PATH,
                                                       introspection_data->interfaces[0],
                                                       &interface_vtable,
                                                       NULL,  /* user_data */
                                                       NULL,  /* user_data_free_func */
                                                       NULL); /* GError** */

  g_assert (registration_id > 0);
}


gboolean
send_data_signal(gpointer connection) 
{
    if (connection == NULL)
        return FALSE;

    GError *error = NULL;
    gchar *test_string = "HelloWorld";
    gdouble test_double = 10.0;
    gint test_int = 1;
    GVariant *data = g_variant_new ("(sdi)",
                                   test_string,
                                   test_double,
                                   test_int);

    g_print ("%s %f %i\n", test_string, test_double, test_int);
    g_dbus_connection_emit_signal (connection,
                                    NULL,
                                    OBJECT_PATH,
                                    INTERFACE_NAME,
                                    "data_signal",
                                    data,
                                    &error);

    if(error != NULL) {
        g_assert(error);
        g_print("%s", error->message);
        g_error_free(error);
        return FALSE;
    }

    return TRUE;
}

gboolean
send_file_signal(gpointer connection) 
{
    if (connection == NULL)
        return FALSE;

    GError *error = NULL;
    gchar *filepath = "/home/debian/test_file.tar.gz";
    GVariant *data = g_variant_new ("(s)", filepath);

    g_print ("%s\n", filepath);
    g_dbus_connection_emit_signal (connection,
                                    NULL,
                                    OBJECT_PATH,
                                    INTERFACE_NAME,
                                    "file_signal",
                                    data,
                                    &error);

    if(error != NULL) {
        g_assert(error);
        g_print("%s", error->message);
        g_error_free(error);
        return FALSE;
    }

    return TRUE;
}

static void
on_name_acquired (GDBusConnection *connection,
                  const gchar     *name,
                  gpointer         user_data)
{
    GMainLoop *loop;
    loop = g_main_loop_new (NULL, FALSE);

    //while(1) {
    //    time(&now);
    //    send_data_signal(connection);
    //    usleep(1000000);
    //}

    if(connection != NULL) {
        g_timeout_add_seconds(1, send_data_signal, connection);
        g_timeout_add_seconds(5, send_file_signal, connection);
    }

    g_main_loop_run (loop);
}

static void
on_name_lost (GDBusConnection *connection,
              const gchar     *name,
              gpointer         user_data)
{
    g_print ("DBus Name failed\n");
    exit (1);
}

int
main (int argc, char *argv[])
{
    guint owner_id;
    GMainLoop *loop = g_main_loop_new (NULL, FALSE);

    introspection_data = g_dbus_node_info_new_for_xml (introspection_xml, NULL);
    g_assert (introspection_data != NULL);

    owner_id = g_bus_own_name (G_BUS_TYPE_SESSION,
                                BUS_NAME,
                                G_BUS_NAME_OWNER_FLAGS_NONE,
                                on_bus_acquired,
                                on_name_acquired,
                                on_name_lost,
                                NULL,
                                NULL);

    g_main_loop_run (loop);

    g_bus_unown_name (owner_id);
    g_dbus_node_info_unref (introspection_data);

    return 0;
}
