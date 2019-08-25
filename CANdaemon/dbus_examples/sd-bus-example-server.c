#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <errno.h>
#include <pthread.h>
#include <systemd/sd-bus.h>


#define INTERFACE_NAME  "org.example.project.oresat"
#define BUS_NAME        INTERFACE_NAME
#define OBJECT_PATH     "/org/example/project/oresat"
#define WAIT_TIME       500000 // mircroseconds


static const sd_bus_vtable method_vtable[];
static sd_bus_slot *slot = NULL;
static sd_bus *bus = NULL;
static bool dbus_running = 0;
static pthread_t method_thread_id;

static void* method_thread(void *arg);


static int dbus_error(char* err, int r) {
    dbus_running = 0;
    fprintf(stderr, "%s %s\n", err, strerror(-r));
    sd_bus_slot_unref(slot);
    sd_bus_unref(bus);
    exit(0);
    return 1;
}


int method_thread_init(void) {
    int r;

    /* Install the object */
    r = sd_bus_add_object_vtable(bus,
                                 &slot,
                                 OBJECT_PATH,
                                 INTERFACE_NAME,
                                 method_vtable,
                                 NULL);
    if (r < 0)
        dbus_error("Failed to issue method call:", -r);
    
    if(pthread_create(&method_thread_id, NULL, method_thread, NULL) != 0)
        dbus_error("method_thread_init - thread creation failed:", 0);

    dbus_running = 1;
    return 1;
}



int method_thread_clear(void) {
    dbus_running = 0;

    /* Wait for thread to finish. */
    if(pthread_join(method_thread_id, NULL) != 0)
        return -1;

    sd_bus_slot_unref(slot);
    sd_bus_unref(bus);
    return 1;
}


static void* method_thread(void *arg) {
    int r;

    while(dbus_running) {
        /* Process requests */
        r = sd_bus_process(bus, NULL);
        if (r < 0)
            dbus_error("Failed to process bus:", -r);
        if (r > 0) /* we processed a request, try to process another one, right-away */
            continue;

        /* Wait for the next request to process */
        r = sd_bus_wait(bus, (uint64_t) -1);
        if (r < 0)
            dbus_error("Failed to wait on bus:", -r);
    }

    return NULL;
}


// ***************************************************************************
// Methods and Method table


static int method_command(sd_bus_message *m, void *userdata, sd_bus_error *ret_error) {
    int r;
    char* command;

    r = sd_bus_message_read(m, "s", &command);
    if (r < 0)
        dbus_error("Failed to parse parameters:", r);

    printf("%s\n", command);

    return sd_bus_reply_method_return(m, "s", "done");
}


static int method_quit(sd_bus_message *m, void *userdata, sd_bus_error *ret_error) {
    dbus_running = 0;
    return sd_bus_reply_method_return(m, "x", 1);
    return 1;
}


static const sd_bus_vtable method_vtable[] = {
        SD_BUS_VTABLE_START(0),
        // key: SD_BUS_METHOD(dbus_method_name, inputs_types, return_types, function_name, SD_BUS_VTABLE_UNPRIVILEGED),
        SD_BUS_METHOD("Quit", NULL, "x", method_quit, SD_BUS_VTABLE_UNPRIVILEGED),
        SD_BUS_METHOD("Command", "s", "s", method_command, SD_BUS_VTABLE_UNPRIVILEGED),
        SD_BUS_VTABLE_END
};


// ***************************************************************************
// Main


int main(int argc, char *argv[]) {
    int r;
    char* test_string = "HelloWorld";
    double test_double = 10.0;
    int32_t test_int = 5;
    
    /* Connect to the user bus */
    r = sd_bus_open_user(&bus);
    if (r < 0)
        dbus_error("Failed to connect to system bus:", -r);

    /* Take a well-known service name so that clients can find us */
    r = sd_bus_request_name(bus, BUS_NAME, 0);
    if (r < 0)
        dbus_error("Failed to acquire service name:", -r);

    method_thread_init();

    while(dbus_running) {
        //send message
        r = sd_bus_emit_signal(bus, 
                               OBJECT_PATH, 
                               INTERFACE_NAME,
                               "data_signal", 
                               "sdi", 
                               test_string,
                               test_double,
                               test_int);
        if (r < 0)
            dbus_error("Signal message failed:", r);

        usleep(WAIT_TIME);
    }

    method_thread_clear();
    return 1;
}

