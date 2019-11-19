#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <errno.h>
#include <systemd/sd-bus.h>


#define INTERFACE_NAME  "org.example.project.oresat"
#define BUS_NAME        INTERFACE_NAME
#define OBJECT_PATH     "/org/example/project/oresat"


/* static data */
static sd_bus_slot *slot = NULL;
static sd_bus *bus = NULL;
static bool endProgram = 0;


/*****************************************************************************/


void dbusError(int r, char* err) {
    if (r < 0)
        fprintf(stderr, "%s %s\n", err, strerror(-r));
    return;
}


void dbusErrorExit(int r, char* err) {
    if (r < 0) {
        fprintf(stderr, "%s %s\n", err, strerror(-r));
        exit(0);
    }
    return;
}


/*****************************************************************************/
/* client */


int call_hello_method(sd_bus *bus) {
    sd_bus_error error = SD_BUS_ERROR_NULL;
    sd_bus_message *m = NULL;
    char *return_string;
    int r;

    printf("Calling Hello method\n");

    if(bus == NULL)
        return -1;

    r = sd_bus_call_method(bus,
                           BUS_NAME,
                           OBJECT_PATH,
                           INTERFACE_NAME,
                           "Hello",
                           &error,
                           &m,
                           "s",
                           "Hello Server!");
    dbusError(r, "Failed to issue Hello method call.");
    
    /* Parse the response message */
    r = sd_bus_message_read(m, "s", &return_string);
    dbusError(r, "Failed to parse response message:");

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

    r = sd_bus_call_method(bus,
                           BUS_NAME,
                           OBJECT_PATH,
                           INTERFACE_NAME,
                           "Multiply",
                           &error,
                           &m,
                           "ii",
                           14, 
                           23);
    dbusError(r, "Failed to issue Multiply method call.");
    
    /* Parse the response message */
    r = sd_bus_message_read(m, "i", &return_int);
    dbusError(r, "Failed to parse response message.");

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

    r = sd_bus_call_method(bus,
                           BUS_NAME,
                           OBJECT_PATH,
                           INTERFACE_NAME,
                           "Quit",
                           &error,
                           &m,
                           NULL);
    dbusError(r, "Failed to issue Quit method call.");
    
    sd_bus_error_free(&error);
    sd_bus_message_unref(m);
    return 0;
}


int client(void) {
    sd_bus *bus = NULL;
    int r;

    /* Connect to the bus */
    r = sd_bus_open_system(&bus);
    dbusError(r, "Failed to connect to system bus:");

    /* Call methods */

    r = call_hello_method(bus);
    dbusError(r, "Hello Method call failed");

    r = call_multiply_method(bus);
    dbusError(r, "Multiply Method call failed");

    r = call_quit_method(bus);
    dbusError(r, "Quit Method call failed");

    sd_bus_unref(bus);
    return 1;
}


/*****************************************************************************/
/* Server */


static int method_hello(sd_bus_message *m, void *userdata, sd_bus_error *ret_error) {
    int r;
    char* input;

    r = sd_bus_message_read(m, "s", &input);
    dbusError(r, "Failed to parse parameters:");

    printf("Hello method called.\n");
    printf("Recieved: %s.\n", input);

    return sd_bus_reply_method_return(m, "s", "done");
}


static int method_multiply(sd_bus_message *m, void *userdata, sd_bus_error *ret_error) {
    int r;
    int32_t input1, input2, output;

    r = sd_bus_message_read(m, "ii", &input1, &input2);
    dbusError(r, "Failed to parse parameters:");

    output = input1 * input2;
    printf("Hello method called.\n");
    printf("Recieved: %d %d.\n", input1, input2);

    return sd_bus_reply_method_return(m, "i", output);
}


static int method_quit(sd_bus_message *m, void *userdata, sd_bus_error *ret_error) {
    endProgram = 1;
    return sd_bus_reply_method_return(m, NULL);
}


static const sd_bus_vtable method_vtable[] = {
    SD_BUS_VTABLE_START(0),
    // key: SD_BUS_METHOD(dbus_method_name, inputs_types, return_types, callback_function, flag),
    SD_BUS_METHOD("Hello", "s", "s", method_hello, SD_BUS_VTABLE_UNPRIVILEGED),
    SD_BUS_METHOD("Multiply", "ii", "i", method_multiply, SD_BUS_VTABLE_UNPRIVILEGED),
    SD_BUS_METHOD("Quit", NULL, NULL, method_quit, SD_BUS_VTABLE_UNPRIVILEGED),
    SD_BUS_VTABLE_END
};


int server(void) {
    int r;
    
    /* Connect to the bus */
    r = sd_bus_open_system(&bus);
    dbusError(r, "Failed to connect to system bus:");

    /* Take a well-known service name so that clients can find us */
    r = sd_bus_request_name(bus, BUS_NAME, SD_BUS_NAME_ALLOW_REPLACEMENT);
    dbusError(r, "Failed to acquire service name. \nIs "INTERFACE_NAME".conf in /etc/dbus-1/system.d/ ?");

    /* Install the vtable */
    r = sd_bus_add_object_vtable(bus,
                                 &slot,
                                 OBJECT_PATH,
                                 INTERFACE_NAME,
                                 method_vtable,
                                 NULL);
    dbusError(r, "Failed to add vtable.");

    while(endProgram == 0) {
        /* Process requests */
        r = sd_bus_process(bus, NULL);
        dbusError(r, "Failed to process bus.");

        if (r > 0) /* we processed a request, try to process another one, right-away */
            continue;

        /* Wait for the next request to process */
        r = sd_bus_wait(bus, (uint64_t) -1);
        dbusError(r, "Failed to wait on bus.");
    }

    r = sd_bus_release_name(bus, BUS_NAME);
    dbusError(r, "Failed to release service name.");

    sd_bus_slot_unref(slot);
    sd_bus_unref(bus);
    return 1;
}


/*****************************************************************************/
/* main */


static void print_input_error(void) {
    printf("Input error \nsudo ./sd-dbus-method-example <Mode> \nWhere <Mode> is server or client.\n");
}


int main(int argc, char *argv[]) {
    int ret = 0;

    if(argc != 2) {
        print_input_error();
        return 1;
    }

    if(strncmp(argv[1], "server", strlen(argv[1])) == 0)
        ret = server();
    else if(strncmp(argv[1], "client", strlen(argv[1])) == 0)
        ret = client();
    else
        print_input_error();

    return ret;
}

