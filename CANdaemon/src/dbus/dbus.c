/*
 * DBUS
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <endian.h>
#include <errno.h>
#include <pthread.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <gio/gio.h>
#include <gio/gunixfdlist.h>

#include "CANopen.h"
#include "dbus.h"
#include "CO_comm_helpers.h"
#include "CO_master.h"


/* Maximum size of Object Dictionary variable transmitted via SDO. */
#define CD_SDO_BUFFER_SIZE     100000 // TODO move to config.h

#define STRING_BUFFER_SIZE  (CD_SDO_BUFFER_SIZE * 4 + 100)
#define LISTEN_BACKLOG      50


/* Variables */
static void*                signal_thread(void* arg);
static pthread_t            signal_thread_id;
static void                 command_process(int fd, char* command, size_t commandLength);
static uint16_t             comm_net = 1;   /* default CAN net number */
static uint8_t              comm_node_default = 0xFF;  /* CANopen Node ID number is undefined at startup. */
static uint16_t             SDOtimeoutTime = 500; /* Timeout time for SDO transfer in milliseconds, if no response */
static uint8_t              blockTransferEnable = 0; /* SDO block transfer enabled? */
static volatile int         endProgram = 0;


/******************************************************************************/
int CO_command_init(void) {

    // TODO code here

    /* Create thread */
    endProgram = 0;
    if(pthread_create(&signal_thread_id, NULL, signal_thread, NULL) != 0) {
        CO_errExit("CO_command_init - thread creation failed");
    }

    return 0;
}


/******************************************************************************/
int CO_command_clear(void) {

    // TODO code here

    endProgram = 1;

    /* Wait for thread to finish. */
    if(pthread_join(signal_thread_id, NULL) != 0) {
        return -1;
    }

    return 0;
}


/******************************************************************************/
static void* signal_thread(void* arg) {
    int fd;
    ssize_t n;
    char buf[STRING_BUFFER_SIZE];
    GMainLoop *loop = g_main_loop_new (NULL, FALSE);
    
    guint watcher_id = g_bus_watch_name (G_BUS_TYPE_SESSION,
                                    DBUS_INTERFACE,
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


/******************************************************************************/
static void command_process(int fd, char* command, size_t commandLength) {
    int err = 0; /* syntax or other error, true or false */
    int emptyLine = 0;
    char *token;
    int i;
    uint32_t ui[3];
    uint8_t comm_node = 0xFF; /* undefined */

    char resp[STRING_BUFFER_SIZE];
    int respLen = 0;
    respErrorCode_t respErrorCode = respErrorNone;

    uint32_t sequence = 0;


    /* Download SDO command - w[rite] <index> <subindex> <datatype> <value> */
    uint16_t idx;
    uint8_t subidx;
    const dataType_t *datatype;
    uint32_t SDOabortCode = 1;

    uint8_t dataTx[CD_SDO_BUFFER_SIZE]; /* SDO transmit buffer */
    uint32_t dataTxLen = 0;  /* Length of data to transmit. */

    token = getTok(NULL, spaceDelim, &err);
    idx = (uint16_t)getU32(token, 0, 0xFFFF, &err);

    token = getTok(NULL, spaceDelim, &err);
    subidx = (uint8_t)getU32(token, 0, 0xFF, &err);

    token = getTok(NULL, spaceDelim, &err);
    datatype = getDataType(token, &err);

    if(err == 0) {
        /* take whole string or single token, depending on datatype. Comment may follow. */
        token = getTok(NULL, (datatype->length == 0) ? "\n\r\f" : spaceDelim, &err);
    }

    if(err == 0) {
        dataTxLen = datatype->dataTypeScan((char*)dataTx, sizeof(dataTx), token);

        /* Length must match and must not be zero. */
        if((datatype->length != 0 && datatype->length != dataTxLen) || dataTxLen == 0) {
            err = 1;
        }
    }

    lastTok(NULL, spaceDelim, &err);

    if(err == 0 && (comm_node < 1 || comm_node > 127)) {
        err = 1;
        if(comm_node == 0xFF) {
            respErrorCode = respErrorNoDefaultNodeSet;
        } else {
            respErrorCode = respErrorUnsupportedNode;
        }
    }

    /* Make CANopen SDO transfer */
    if(err == 0) {
        err = sdoClientDownload(
                CO->SDOclient,
                comm_node,
                idx,
                subidx,
                dataTx,
                dataTxLen,
                &SDOabortCode,
                SDOtimeoutTime,
                blockTransferEnable);

        if(err != 0){
            respErrorCode = respErrorInternalState;
        }
    }

    /* output result */
    if(err == 0){
        if(SDOabortCode == 0) {
            respLen = sprintf(resp, "[%d] OK", sequence);
        }
        else{
            respLen = sprintf(resp, "[%d] ERROR: 0x%08X", sequence, SDOabortCode);
        }



    /* Generate error response (or leave empty line response) */
    if(err != 0 && emptyLine == 0) {
        if(respErrorCode == respErrorNone) {
            respErrorCode = respErrorSyntax;
        }
        respLen = sprintf(resp, "[%d] ERROR: %d", sequence, respErrorCode);
    }


    /* Terminate string and send response */
    resp[respLen++] = '\r';
    resp[respLen++] = '\n';
    resp[respLen++] = '\0';

    if(write(fd, resp, respLen) != respLen) {
        CO_error(0x15200000L);
    }
}


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
    GMainLoop *loop = g_main_loop_new (NULL, FALSE);

    g_variant_get (parameters,
                   "(&sd)",
                   &test_string,
                   &test_double);

    g_print ("%s %f\n", 
                test_string, 
                test_double);

    // TODO call SDO sender

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

    // TODO call SDO sender
    //
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
                                           DBUS_INTERFACE,
                                           DBUS_INTERFACE,
                                           "data_signal",
                                           DBUS_PATH,
                                           NULL,
                                           G_DBUS_SIGNAL_FLAGS_NONE,
                                           read_data_signal,
                                           user_data,
                                           NULL);

    // subscribe to file signal
    s2 = g_dbus_connection_signal_subscribe (connection,
                                           DBUS_INTERFACE,
                                           DBUS_INTERFACE,
                                           "file_signal",
                                           DBUS_PATH,
                                           NULL,
                                           G_DBUS_SIGNAL_FLAGS_NONE,
                                           read_file_signal,
                                           user_data,
                                           NULL);

    g_main_loop_run (loop);

    g_dbus_connection_signal_unsubscribe (connection, s1);
    g_dbus_connection_signal_unsubscribe (connection, s2);
}

