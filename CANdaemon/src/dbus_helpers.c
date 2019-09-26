#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

#include "CANopen.h"
#include "CO_SDO.h"
#include "CO_driver.h"
#include "CO_OD.h"
#include "CO_master.h"
#include "dbus_helpers.h"
#include "CO_comm_helpers.h"


#define FILENAME_MAX_LENGTH 20


/* Variables */
static uint16_t             SDOtimeoutTime = 500; /* Timeout time for SDO transfer in milliseconds, if no response */
static uint8_t              blockTransferEnable = 1; /* SDO block transfer enabled? */

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


int OD_add_file(const uint16_t idx, 
                const uint8_t subidx_name, 
                const uint8_t subidx_data,
                const char* file_path) {

    if(file_path == NULL)
        return 0;

    int file_size = get_file_size(file_path);
    char* file_name = remove_path(file_path);
    char* file_data = read_file(file_path, file_size);

    if((file_name == NULL) || (file_data == NULL)) {
        printf("Invalid file path\n");
        return -1;
    }

    OD_setData(idx, subidx_name, file_name, strlen(file_name)); // don't send '\0'
    OD_setData(idx, subidx_data, file_data, file_size);

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

    if(size_new > FILENAME_MAX_LENGTH) 
        return NULL;

    // make the filename of all spaces
    file_name = (char *)malloc(FILENAME_MAX_LENGTH+1);
    for(unsigned int i=0; i<FILENAME_MAX_LENGTH; ++i) 
        file_name[i] = ' ';

    // copy only file name
    strncpy(file_name, &file_path[start], size_new-1);
    file_name[FILENAME_MAX_LENGTH] = '\0';

    return file_name;
}


int OD_setData(uint16_t idx, uint8_t subidx, void *input_data, const uint32_t dataTxLen) {
    if(input_data == NULL)
        return -1;

    int err = 0; /* syntax or other error, true or false */
    respErrorCode_t respErrorCode = respErrorNone;
    uint32_t SDOabortCode = 1;
    uint8_t dataTx[CO_COMMAND_SDO_BUFFER_SIZE]; /* SDO transmit buffer */

    /* Length must match and must not be zero. */
    if(dataTxLen == 0 || dataTxLen >= CO_COMMAND_SDO_BUFFER_SIZE)
        return -1;

    memcpy(dataTx, input_data, dataTxLen);

    /* Make CANopen SDO transfer */
    err = sdoClientDownload(
            CO->SDOclient[0],
            OD_CANNodeID,
            idx,
            subidx,
            dataTx,
            dataTxLen,
            &SDOabortCode,
            SDOtimeoutTime,
            blockTransferEnable);

    if(err != 0){
        respErrorCode = respErrorInternalState;
        printf("SDO upload err: %d\n", respErrorCode);
        return -1;
    }

    return 0;
}


// TODO handle time of day and time differences


int OD_getNonArrayData(const uint16_t idx, const uint8_t subidx, void *dataOut, const int dataLength) {
    if(dataOut == NULL)
        return -1;

    int err = 0; /* syntax or other error, true or false */
    respErrorCode_t respErrorCode = respErrorNone;
    uint32_t SDOabortCode = 1;
    uint8_t dataRx[CO_COMMAND_SDO_BUFFER_SIZE]; /* SDO receive buffer */
    uint32_t dataRxLen;  /* Length of received data */

    /* Make CANopen SDO transfer */
    err = sdoClientUpload(
        CO->SDOclient[0],
        OD_CANNodeID,
        idx,
        subidx,
        dataRx,
        sizeof(dataRx),
        &dataRxLen,
        &SDOabortCode,
        SDOtimeoutTime,
        blockTransferEnable);


    if(err != 0){
        respErrorCode = respErrorInternalState;
        printf("SDO upload err: %d\n", respErrorCode);
        return -1;
    }

    if(dataRxLen != dataLength)
        return -1;

    memcpy(&dataOut, &dataRx, dataRxLen);

    return 0;
}

int OD_getArrayData(const uint16_t idx, const uint8_t subidx, char *dataOut, int32_t *dataLength) {
    int err = 0; /* syntax or other error, true or false */
    respErrorCode_t respErrorCode = respErrorNone;
    uint32_t SDOabortCode = 1;
    uint8_t dataRx[CO_COMMAND_SDO_BUFFER_SIZE]; /* SDO receive buffer */
    uint32_t dataRxLen;  /* Length of received data */

    if(dataOut != NULL)
        return -1;

    /* Make CANopen SDO transfer */
    err = sdoClientUpload(
        CO->SDOclient[0],
        OD_CANNodeID,
        idx,
        subidx,
        dataRx,
        sizeof(dataRx),
        &dataRxLen,
        &SDOabortCode,
        SDOtimeoutTime,
        blockTransferEnable);


    if(err != 0){
        respErrorCode = respErrorInternalState;
        printf("SDO upload err: %d\n", respErrorCode);
        return -1;
    }

    if(dataRxLen == 0)
        return -1;

    (*dataLength) = dataRxLen;
    dataOut = (char *)malloc(dataRxLen);
    memcpy(&dataOut, &dataRx, dataRxLen);

    return 0;
}


CO_SDO_abortCode_t file_transfer(CO_ODF_arg_t *ODF_arg) {
    CO_OD_file_data_t *odFileData;
    CO_SDO_abortCode_t ret = CO_SDO_AB_NONE;

    odFileData = (CO_OD_file_data_t*) ODF_arg->object;

    if(ODF_arg->subIndex != 2) {
        // nothing special
        return ret;
    }

    if(ODF_arg->reading) { 
        /* read parameters */
        if(odFileData->fileData == NULL || odFileData->fileSize <= 0) {
            //error, no data to read
            ret = CO_SDO_AB_NO_DATA; 
            return ret;
        }

        if(ODF_arg->offset == 0) {
            /* 1st offset */
            ODF_arg->dataLengthTotal = odFileData->fileSize;
        }

        memcpy(ODF_arg->data, &(odFileData->fileData[ODF_arg->offset]), ODF_arg->dataLength);
        ++ODF_arg->offset;
    }
    else { 
        /* store parameters */
        if(ODF_arg->offset == 0) {
            /* 1st offset */
            if(odFileData->fileData != NULL) {
                /* clear data for new transfer */
                free(odFileData->fileData);
            }

            /* allocate memory for new file */
            odFileData->fileSize = ODF_arg->dataLengthTotal;
            odFileData->fileData = (uint8_t *)malloc(odFileData->fileSize);
        }

        memcpy(&(odFileData->fileData[ODF_arg->offset]), ODF_arg->data, ODF_arg->dataLength);
        ++ODF_arg->offset;
    }

    return ret;
}


CO_ReturnError_t CO_OD_file_transfer_init(CO_OD_file_data_t *odFileData) {
    odFileData->fileData = NULL;
    odFileData->fileSize = 0;
    return CO_ERROR_NO;
}


void CO_OD_file_transfer_close(CO_OD_file_data_t *odFileData) {
    if(odFileData->fileData != NULL) {
        free(odFileData->fileData);
        odFileData = NULL;
    }
    return;
}
