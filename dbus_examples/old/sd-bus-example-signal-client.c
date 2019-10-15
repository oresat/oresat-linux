#include <stdio.h>
#include <stdlib.h>
#include <systemd/sd-bus.h>


#define INTERFACE_NAME  "org.example.project.oresat"
#define BUS_NAME        INTERFACE_NAME
#define OBJECT_PATH     "/org/example/project/oresat"
//#define USER_DBUS       1 // comment out if system dbus is wanted


static int dbus_error(char* err, int r) {
    fprintf(stderr, "%s %s\n", err, strerror(-r));
    exit(0);
    return 1;
}


// callback for reading the data signal
static int read_data_signal_cb(sd_bus_message *m, void *user_data, sd_bus_error *ret_error) {
    int r;
    char *message_string = NULL;
    double message_double = 0.0;
    int32_t message_int = 0;

    r = sd_bus_message_read(m, "sdi", &message_string, &message_double, &message_int);
    if(r < 0) {
        fprintf(stderr, "Failed to parse data signal %s\n", strerror(-r));
        return -1;
    }

    fprintf(stderr, "%s %f %d\n", message_string, message_double, message_int);

    //sd_bus_message_unref(m);
    return 0;
}


int main(int argc, char *argv[]) {
    sd_bus_slot *slot = NULL;
    sd_bus *bus = NULL;
    int r;

    /* Connect to the bus */
#ifdef USER_DBUS
    r = sd_bus_open_user(&bus);
    if(r < 0)
        dbus_error("Failed to connect to user bus:", r);
#else
    r = sd_bus_open_system(&bus);
    if (r < 0)
        dbus_error("Failed to connect to system bus:", r);
#endif

    r = sd_bus_add_match(bus,
                         &slot,
                         "type='signal', path='"OBJECT_PATH"', interface='"INTERFACE_NAME"', member='data_signal'", 
                         read_data_signal_cb, 
                         NULL);
    if(r < 0)
        dbus_error("Add match error:", r);

    for(;;) {
        r = sd_bus_process(bus, NULL);
        if(r < 0)
            dbus_error("Bus Process failed:", r);
        if(r > 0)
            continue;

        r = sd_bus_wait(bus, (uint64_t)-1);
        if(r < 0)
            dbus_error("Bus wait failed:", r);
    }

    sd_bus_slot_unref(slot);
    sd_bus_unref(bus);

    return 1;
}
