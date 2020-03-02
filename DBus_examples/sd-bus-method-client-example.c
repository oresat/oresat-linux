#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <errno.h>
#include <systemd/sd-bus.h>


#define DESTINATION     "org.OreSat.Example"
#define INTERFACE_NAME  "org.OreSat.Example"
#define OBJECT_PATH     "/org/OreSat/Example"


/*****************************************************************************/
/* Since all dbus function return negative errno values on failure,
 * these functionare just useful easy assert functions.
 */


void dbus_assert_error(int r, char* err) {
    if (r < 0)
        fprintf(stderr, "%s %s\n", err, strerror(-r));
    return;
}


void dbus_assert_error_exit(int r, char* err) {
    if (r < 0) {
        fprintf(stderr, "%s %s\n", err, strerror(-r));
        exit(0);
    }
    return;
}


int call_hello_method(sd_bus *bus) {
    sd_bus_error error = SD_BUS_ERROR_NULL;
    sd_bus_message *m = NULL;
    char *return_string;
    int r;

    printf("Calling Hello method\n");

    if(bus == NULL)
        return -1;

    r = sd_bus_call_method(bus,
                           DESTINATION,
                           OBJECT_PATH,
                           INTERFACE_NAME,
                           "Hello",
                           &error,
                           &m,
                           "s",
                           "Hello Server!");
    dbus_assert_error(r, "Failed to issue Hello method call.");

    /* Parse the response message */
    r = sd_bus_message_read(m, "s", &return_string);
    dbus_assert_error(r, "Failed to parse response message:");

    printf("Returned: %s\n\n", return_string);

    sd_bus_error_free(&error);
    sd_bus_message_unref(m);
    return 0;
}


int call_multiply_method(sd_bus *bus) {
    sd_bus_error error = SD_BUS_ERROR_NULL;
    sd_bus_message *m = NULL;
    int32_t return_int;
    int r;

    printf("Calling Multiply method\n");

    if(bus == NULL)
        return -1;

    r = sd_bus_call_method(
            bus,
            DESTINATION,
            OBJECT_PATH,
            INTERFACE_NAME,
            "Multiply",
            &error,
            &m,
            "ii",
            14,
            23);
    dbus_assert_error(r, "Failed to issue Multiply method call.");

    // Parse the response message
    r = sd_bus_message_read(m, "i", &return_int);
    dbus_assert_error(r, "Failed to parse response message.");

    printf("Returned: %d\n\n", return_int);

    sd_bus_error_free(&error);
    sd_bus_message_unref(m);
    return 0;
}


int call_quit_method(sd_bus *bus) {
    sd_bus_error error = SD_BUS_ERROR_NULL;
    sd_bus_message *m = NULL;
    int r;

    printf("Quit Method Called\n");

    if(bus == NULL)
        return -1;

    r = sd_bus_call_method(
            bus,
            DESTINATION,
            OBJECT_PATH,
            INTERFACE_NAME,
            "Quit",
            &error,
            &m,
            NULL);
    dbus_assert_error(r, "Failed to issue Quit method call.");

    sd_bus_error_free(&error);
    sd_bus_message_unref(m);
    return 0;
}


int main(int argc, char *argv[]) {
    sd_bus *bus = NULL;
    int r;

    /* Connect to the bus */
    r = sd_bus_open_system(&bus);
    dbus_assert_error(r, "Failed to connect to system bus:");

    /* Call methods */

    r = call_hello_method(bus);
    dbus_assert_error(r, "Hello Method call failed");

    r = call_multiply_method(bus);
    dbus_assert_error(r, "Multiply Method call failed");

    r = call_quit_method(bus);
    dbus_assert_error(r, "Quit Method call failed");

    sd_bus_unref(bus);
    return 1;
}


