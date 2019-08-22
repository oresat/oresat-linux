#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

#include "configs.h"
#include "stack/CANopen.h"
#include "CO_master.h"
#include "dbus_helpers.h"
#include "CO_comm_helpers.h"


/* Variables */
static uint16_t             SDOtimeoutTime = 500; /* Timeout time for SDO transfer in milliseconds, if no response */
static uint8_t              blockTransferEnable = 1; /* SDO block transfer enabled? */


int send_file(const char* file_path, 
              const uint16_t idx, 
              const uint8_t subidx_name, 
              const uint8_t subidx_data) {

    if(file_path == NULL)
        return 0;

    int file_size = get_file_size(file_path);
    char* file_name = remove_path(file_path);
    char* file_data = read_file(file_path, file_size);

    if((file_name == NULL) || (file_data == NULL)) {
        printf("Invalid file path\n");
        return -1;
    }

    send_SDO(idx, subidx_name, file_name, strlen(file_name)+1);
    send_SDO(idx, subidx_data, file_data, file_size);
    return 1; 
}


unsigned int get_file_size(const char* file_path) {
    if(file_path == NULL)
        return 0;

    size_t file_size;
    FILE *f = fopen(file_path, "r");

    if(f == NULL) {
        printf("File not found: %s\n", file_path); 
        return 0;
    }

    // get file size
    fseek(f, 0, SEEK_END);
    file_size = ftell(f);
    fclose(f);

    return file_size;
}


char* read_file(const char* file_path, unsigned int file_size) {
    if(file_path == NULL)
        return NULL;

    char * file_data = NULL;
    FILE *f = fopen(file_path, "r");

    if(f == NULL) {
        printf("File not found: %s\n", file_path); 
        return NULL;
    }

    // read into buffer
    file_data = malloc((file_size + 1) * sizeof(*file_data));
    fread(file_data, file_size, 1, f);
    file_data[file_size] = '\0';

    fclose(f);

    return file_data;
}


char* remove_path(const char* file_path) {
    if(file_path == NULL)
        return NULL;

    char * file_name = NULL;
    unsigned int size;
    unsigned int size_new;
    unsigned int start;

    size = strlen(file_path) + 1;
    size_new = size;
    start = size-1;
    
    // find right most '/'
    while(start > 0) {
        if(file_path[start] == '/') {
            ++start;
            size_new = size - start;
            break;
        }
        --start;
    }

    // copy only file name
    file_name = (char *)malloc(size_new);
    strncpy(file_name, &file_path[start], size_new);

    return file_name;
}


void send_SDO(uint16_t idx, uint8_t subidx, char* input_data, uint32_t len) {
    if(input_data == NULL) {
        printf("Input error\n");
        return;
    }

    int err = 0; /* syntax or other error, true or false */
    int emptyLine = 0;
    char resp[STRING_BUFFER_SIZE];
    int respLen = 0;
    respErrorCode_t respErrorCode = respErrorNone;
    uint32_t sequence = 0;
    //const dataType_t *datatype;
    uint32_t SDOabortCode = 1;
    uint8_t dataTx[SDO_BUFFER_SIZE]; /* SDO transmit buffer */
    uint32_t dataTxLen = 0;  /* Length of data to transmit. */

    dataTxLen = len;
    /* Length must match and must not be zero. */
    if(dataTxLen == 0 || dataTxLen >= SDO_BUFFER_SIZE) {
        printf("len error\n");
        err = 1;
    }
    memcpy(dataTx, input_data, dataTxLen);

    if(NODE_ID < 1 || NODE_ID > 127) {
        err = 1;
        respErrorCode = respErrorUnsupportedNode;
        printf("node id err: %d\n", respErrorCode);
    }

    printf("sending SDO\n");

    /* Make CANopen SDO transfer */
    if(err == 0) {
        err = sdoClientDownload(
                CO->SDOclient,
                NODE_ID,
                idx,
                subidx,
                dataTx,
                dataTxLen,
                &SDOabortCode,
                SDOtimeoutTime,
                blockTransferEnable);

        if(err != 0){
            respErrorCode = respErrorInternalState;
            printf("SDO download err: %d\n", respErrorCode);
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

    printf("SDO err: %d\n", err);

    /* TODO printf or log
    if(write(fd, resp, respLen) != respLen) {
        CO_error(0x15200000L);
    }
    */
}
