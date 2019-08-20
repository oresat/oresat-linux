#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

#include "dbus_helpers.h"
#include "CO_comm_helpers.h"


void send_file(char* file_path, 
                uint16_t idx_name, 
                uint8_t subidx_name, 
                uint16_t idx_data, 
                uint8_t subidx_data) {

    if(file_path == NULL)
        return;

    char* file_name = NULL;
    char* file_data = NULL;
    
    int isValid1 = read_file(file_path, file_data);
    int isValid2 = remove_path(file_path, file_name);

    if(isValid1 && isValid2) {
        send_SDO(idx_name, subidx_name, file_name);
        send_SDO(idx_data, subidx_data, file_data);
    }

    return; 
}


int read_file(char* file_path, char* file_data) {
    if(file_path == NULL)
        return -1;

    if(file_data != NULL) {
        free(file_data);
        file_data = NULL;
    }

    size_t file_size;
    FILE *f = fopen(file_path, "r");

    if(f == NULL)
        return -1;

    // get file size
    fseek(f, 0, SEEK_END);
    file_size = ftell(f);
    rewind(f);

    // read into buffer
    file_data = malloc((file_size + 1) * sizeof(*file_data));
    fread(file_data, file_size, 1, f);
    file_data[file_size] = '\0';

    fclose(f);

    return 1;
}


int remove_path(char* file_path, char* file_name) {
    if(file_path == NULL)
        return -1;

    size_t size = strlen(file_path)+1;
    size_t size_new = size;
    unsigned int start;

    if(file_name != NULL) {
        free(file_name);
        file_name = NULL;
    }
    
    // find right most '/'
    for(start=size; start>0; --start) {
        if(file_path[start] == '/') {
            size_new = size - start;
            ++start; // start after 1st '/'
            break;
        }
    }

    // copy only file name
    file_name = malloc((size_new) * sizeof(*file_path));
    strncpy(file_name, &(file_path[start]), size_new);

    printf("Filename: %s", file_name); // TODO remove

    return 1;
}

void send_SDO(uint16_t idx, uint8_t subidx, char* input_data) {
    int err = 0; /* syntax or other error, true or false */
    int emptyLine = 0;
    char *token;
    char resp[STRING_BUFFER_SIZE];
    int respLen = 0;
    respErrorCode_t respErrorCode = respErrorNone;
    uint32_t sequence = 0;
    const dataType_t *datatype;
    uint32_t SDOabortCode = 1;
    uint8_t dataTx[SDO_BUFFER_SIZE]; /* SDO transmit buffer */
    uint32_t dataTxLen = 0;  /* Length of data to transmit. */

    /* Download SDO command - w[rite] <index> <subindex> <datatype> <value> */

    datatype = getDataType("vs", &err); // vs for visual string

    if(err == 0) {
        dataTxLen = datatype->dataTypeScan((char*)dataTx, sizeof(dataTx), input_data);

        /* Length must match and must not be zero. */
        if((datatype->length != 0 && datatype->length != dataTxLen) || dataTxLen == 0) {
            err = 1;
        }
    }

    if(NODE_ID < 1 || NODE_ID > 127) {
        err = 1;
        respErrorCode = respErrorUnsupportedNode;
    }

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

    /* TODO printf
    if(write(fd, resp, respLen) != respLen) {
        CO_error(0x15200000L);
    }
    */
}
