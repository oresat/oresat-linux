#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <stddef.h>
#include <errno.h>
#include <pthread.h>
#include <systemd/sd-bus.h>

#include "gps-dbus-server.h"

#define INTERFACE_NAME  "org.OreSat.GPS"
#define BUS_NAME        INTERFACE_NAME
#define OBJECT_PATH     "/org/OreSat/GPS"
#define WAIT_TIME       500000 // mircroseconds

static const sd_bus_vtable vtable[];
static sd_bus *bus = NULL;
static bool endProgram = 0;
static pthread_t vtable_thread_id;
static struct gps_status status;
static int wait_time = WAIT_TIME;

static void* vtable_thread(void *arg);


static void dbus_assert(const int r, const char* err) {
    if (r < 0)
        fprintf(stderr, "%s %s\n", err, strerror(-r));
    return;
}


int vtable_thread_init(void) {
    endProgram = 0;
    status.current_state = eRunningHighPower;
    if(pthread_create(&vtable_thread_id, NULL, vtable_thread, NULL) != 0)
        fprintf(stderr, "vtable_thread_init - thread creation failed");

    return 1;
}


int vtable_thread_clear(void) {
    endProgram = 1;

    /* Wait for thread to finish. */
    if(pthread_join(vtable_thread_id, NULL) != 0)
        return -1;

    return 1;
}


static void* vtable_thread(void *arg) {
    int r;

    while(endProgram == 0) {
        /* Process requests */
        r = sd_bus_process(bus, NULL);
        dbus_assert(r, "Failed to process bus.");
        if (r > 0) /* we processed a request, try to process another one, right-away */
            continue;

        /* Wait for the next request to process */
        r = sd_bus_wait(bus, (uint64_t) -1);
        dbus_assert(r, "Failed to wait on bus.");
    }

    return NULL;
}


/****************************************************************************/
/* Methods and Method table */


static int change_state(sd_bus_message *m, void *systemdata, sd_bus_error *ret_error) {
    int r;
    int32_t new_state;
    r = sd_bus_message_read(m, "i", &new_state);
    dbus_assert(r, "Failed to parse parameters:");
    
    printf("chage state input %i.\n", new_state);

    switch(new_state) {
        case eRunningHighPower :
            status.current_state = eRunningHighPower;
            wait_time = WAIT_TIME;
            break;
        case eRunningLowPower :
            status.current_state = eRunningLowPower;
            wait_time = 10000000;
            break;
        case eExit :
            status.current_state = eExit;
            endProgram = 1;
            break;
        default :
            dbus_assert(-1, "Unkown State.");
            break;
    }
    return sd_bus_reply_method_return(m, "i", 1);
}


static const sd_bus_vtable vtable[] = {
        SD_BUS_VTABLE_START(0),
        /* key: SD_BUS_METHOD(dbus_method_name, inputs_types, return_types, function_name, flag), */
        SD_BUS_METHOD("ChangeState", "i", "i", change_state, SD_BUS_VTABLE_UNPRIVILEGED),
	SD_BUS_PROPERTY("PositionX", "n", NULL, offsetof(struct stateVector, posX), 0),
	SD_BUS_PROPERTY("PositionY", "n", NULL, offsetof(struct stateVector, posY), 0),
	SD_BUS_PROPERTY("PositionZ", "n", NULL, offsetof(struct stateVector, posZ), 0),
	SD_BUS_PROPERTY("VelocityX", "n", NULL, offsetof(struct stateVector, velX), 0),
	SD_BUS_PROPERTY("VelocityY", "n", NULL, offsetof(struct stateVector, velY), 0),
	SD_BUS_PROPERTY("VelocityZ", "n", NULL, offsetof(struct stateVector, velZ), 0),
	SD_BUS_PROPERTY("AccelerationX", "n", NULL, offsetof(struct stateVector, accX), 0),
	SD_BUS_PROPERTY("AccelerationY", "n", NULL, offsetof(struct stateVector, accY), 0),
	SD_BUS_PROPERTY("AccelerationZ", "n", NULL, offsetof(struct stateVector, accZ), 0),
        SD_BUS_VTABLE_END
};


/***************************************************************************/
/* Main */


int main(int argc, char *argv[]) {
    int r;
    struct stateVector sv;

    if(getuid() != 0) {
        printf("This uses system dbus. Run as root.\n");
        return 1;
    }
    
    /* Connect to the bus */
    r = sd_bus_open_system(&bus);
    dbus_assert(r, "Failed to connect to system bus.");

    /* Install the object */
    r = sd_bus_add_object_vtable(bus,
                                 NULL,
                                 OBJECT_PATH,
                                 INTERFACE_NAME,
                                 vtable,
                                 &sv);
    dbus_assert(r, "Failed to add vtable");

    /* Take a well-known service name so that clients can find us */
    r = sd_bus_request_name(bus, BUS_NAME, SD_BUS_NAME_REPLACE_EXISTING);
    dbus_assert(r, "Failed to acquire service name.");
    if( r < 0)
        exit(1);

    vtable_thread_init();

    while(endProgram == 0) {
        /* just to make random data for now */
        sv.posX = 0;
        ++sv.posY;
        --sv.posZ;;
        sv.velX += 7;
        sv.velY += 5;
        sv.velZ -= 6;
        sv.accX -= 8;
        sv.accY -= 9;
        sv.accZ += 3;

        /* send signal */
        r = sd_bus_emit_signal(bus, 
                               OBJECT_PATH, 
                               INTERFACE_NAME,
                               "StateVectorSignal", 
                               "nnn", 
                               sv.posX,
                               sv.posY,
                               sv.posZ);
        dbus_assert(r, "Signal message failed.");

        /* send signal */
        r = sd_bus_emit_signal(bus, 
                               OBJECT_PATH, 
                               INTERFACE_NAME,
                               "StatusSignal", 
                               "i", 
                                status.current_state);
        dbus_assert(r, "Signal message failed.");

        usleep(wait_time);
    }

    vtable_thread_clear();

    r = sd_bus_release_name(bus, BUS_NAME);
    dbus_assert(r, "Failed to release service name.");
    sd_bus_unref(bus);

    return 0;
}

