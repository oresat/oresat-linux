#include <gio/gio.h>
#include <stdlib.h>
#include <gio/gunixfdlist.h>
#include <unistd.h>

/* ---------------------------------------------------------------------------------------------------- */

static GDBusNodeInfo *introspection_data = NULL;

/* Introspection data for the service we are exporting */
static const gchar introspection_xml[] =
    "<node>"
    "   <interface name='org.example.project_1.server_1'>"
    "       <method name='Command'>"
    "           <arg type='s' name='command' direction='in'/>"
    "           <arg type='s' name='response' direction='out'/>"
    "       </method>"
    "       <method name='Quit'/>"
    "       <signal name='app_1_signal'>"
    "           <arg type='s'/>"
    "           <arg type='d'/>"
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
      gchar *greeting;

      g_variant_get (parameters, "(&s)", &greeting);

      gchar *response;
      response = g_strdup_printf ("You greeted me with '%s'. Thanks!", greeting);
      g_dbus_method_invocation_return_value (invocation,
                                             g_variant_new ("(s)", response));
      g_free (response);
    }
  else if (g_strcmp0 (method_name, "Quit") == 0)
  {
      gchar *response;
      response = g_strdup_printf ("Quit");
      g_dbus_method_invocation_return_value (invocation,
                                             g_variant_new ("(s)", response));
      g_free (response);
  }
}

static gchar *_global_title = NULL;

static gboolean swap_a_and_b = FALSE;

/* for now */
static const GDBusInterfaceVTable interface_vtable =
{
  handle_method_call,
  //this is a list of avaible methods
};


/* ---------------------------------------------------------------------------------------------------- */

static void
on_bus_acquired (GDBusConnection *connection,
                 const gchar     *name,
                 gpointer         user_data)
{
  guint registration_id;

  registration_id = g_dbus_connection_register_object (connection,
                                                       "/org/example/project_1/server_1",
                                                       introspection_data->interfaces[0],
                                                       &interface_vtable,
                                                       NULL,  /* user_data */
                                                       NULL,  /* user_data_free_func */
                                                       NULL); /* GError** */
  g_assert (registration_id > 0);
}

static void
on_name_acquired (GDBusConnection *connection,
                  const gchar     *name,
                  gpointer         user_data)
{
    GMainLoop *loop;
    GError *error;
    gboolean ret;

    g_assert (connection != NULL);
    g_assert (!g_dbus_connection_is_closed (connection));

    gchar *test_string = "HelloWorld";
    gdouble test_double = 10.0;
    GVariant *data = g_variant_new ("(&sd)",
                                   test_string,
                                   test_double);

    g_print ("%s %d\n", 
                test_string, 
                test_double);

    ret = g_dbus_connection_emit_signal (connection,
                                        NULL, /* destination bus name */
                                        "/org/example/project_1/server_1",
                                        "org.example.project_1.server_1",
                                        "app_1_signal",
                                        data,
                                        &error);

    loop = g_main_loop_new (NULL, FALSE);
    g_main_loop_run (loop);

    g_assert_no_error(error);
    g_assert(ret);

}

static void
on_name_lost (GDBusConnection *connection,
              const gchar     *name,
              gpointer         user_data)
{
  exit (1);
}

int
main (int argc, char *argv[])
{
    guint owner_id;
    GMainLoop *loop;

    introspection_data = g_dbus_node_info_new_for_xml (introspection_xml, NULL);
    g_assert (introspection_data != NULL);

    owner_id = g_bus_own_name (G_BUS_TYPE_SESSION,
                                "org.gtk.GDBus.TestServer",
                                G_BUS_NAME_OWNER_FLAGS_NONE,
                                on_bus_acquired,
                                on_name_acquired,
                                on_name_lost,
                                NULL,
                                NULL);

    loop = g_main_loop_new (NULL, FALSE);
    g_main_loop_run (loop);

    g_bus_unown_name (owner_id);
    g_dbus_node_info_unref (introspection_data);

    return 0;
}
