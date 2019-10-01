/*
 *
 */


#include "CANopen.h"
#include "CO_driver.h"
#include "app_OD_functions.h"
#include "app_OD_helpers.h"
#include <string.h>
#include <stdio.h>

pthread_mutex_t APP_ODF_mtx;
#define APP_LOCK_ODF()            {if(pthread_mutex_lock(&APP_ODF_mtx) != 0) CO_errExit("Mutex lock CO_OD_mtx failed");}
#define APP_UNLOCK_ODF()          {if(pthread_mutex_unlock(&APP_ODF_mtx) != 0) CO_errExit("Mutex unlock CO_OD_mtx failed");}


/* Static Variables */
static CO_OD_file_data_t    odSendFileData;     /* Pointer to file data for DOMAIN based OD entry */


void app_ODF_configure(void){
    /* Add data structs to SDO Object Dictionary */
    CO_OD_configure(CO->SDO[0], 0x3002, CO_ODF_3002, (void*)&odSendFileData, 0, 0U);

    return;
}


/******************************************************************************/
/* send file, OD entry 3002 */

CO_SDO_abortCode_t CO_ODF_3002(CO_ODF_arg_t *ODF_arg) {
    CO_OD_file_data_t *odFileData;
    CO_SDO_abortCode_t ret = CO_SDO_AB_NONE;

    odFileData = (CO_OD_file_data_t*) ODF_arg->object;

    APP_LOCK_ODF();

    if(ODF_arg->reading) { /* read parameters */
        if(ODF_arg->subIndex == 1) { /* file size*/
            ODF_arg->lastSegment = true;
            ODF_arg->dataLength = sizeof(odFileData->fileSize);
            ODF_arg->dataLengthTotal = sizeof(odFileData->fileSize);
            memcpy(ODF_arg->data, &(odFileData->fileSize), ODF_arg->dataLength);
        }
        else if(ODF_arg->subIndex == 2) { /* file data */
            if(odFileData->fileSize == 0) /* error, no data to read */
                return CO_SDO_AB_NO_DATA; 

            /* check if new data will fit in struct */
            if(odFileData->fileSize > FILE_TRANSFER_MAX_SIZE)
                return CO_SDO_AB_OUT_OF_MEM; 
            ODF_arg->lastSegment = true;

            ODF_arg->dataLength = odFileData->fileSize;
            ODF_arg->dataLengthTotal = odFileData->fileSize;

            memcpy(ODF_arg->data, &(odFileData->fileData[0]), ODF_arg->dataLength);
        }
        else if(ODF_arg->subIndex == 3) { /* filename size */
            ODF_arg->lastSegment = true;
            ODF_arg->dataLength = sizeof(odFileData->fileNameSize);
            ODF_arg->dataLengthTotal = sizeof(odFileData->fileNameSize);
            memcpy(ODF_arg->data, &(odFileData->fileNameSize), ODF_arg->dataLength);
        }
        else if(ODF_arg->subIndex == 4) { /* filename */
            if(odFileData->fileNameSize == 0) /* error, no data to read */
                return CO_SDO_AB_NO_DATA; 

            /* check if new data will fit in struct */
            if(odFileData->fileNameSize > FILE_TRANSFER_MAX_SIZE)
                return CO_SDO_AB_OUT_OF_MEM; 

            ODF_arg->dataLength = odFileData->fileNameSize;
            ODF_arg->dataLengthTotal = odFileData->fileNameSize;

            ODF_arg->lastSegment = true;
            memcpy(ODF_arg->data, &(odFileData->fileName[0]), ODF_arg->dataLength);
        }
        else 
            return CO_SDO_AB_SUB_UNKNOWN; 
    }
    else 
        return CO_SDO_AB_WRITEONLY; 

    APP_UNLOCK_ODF();

    return ret;
}


uint32_t APP_ODF_3002(const char *filePath) {
    uint32_t ret = 0;

    if(filePath == NULL)
        return 1;
   
    APP_LOCK_ODF();

    ret = get_file_name(filePath, odSendFileData.fileName, &odSendFileData.fileNameSize);
    ret = get_file_data(filePath, odSendFileData.fileData, &odSendFileData.fileSize);
    
    APP_UNLOCK_ODF();

    return ret; 
}


int32_t get_file_name(const char *filePath, int8_t *fileName, uint16_t *fileNameSize) {
    uint32_t ret = 0;
    uint16_t size;
    uint16_t size_new;
    uint16_t start;

    size = strlen(filePath) + 1;
    size_new = size;
    start = size-1;
    
    /* find right most '/' */
    while(start > 0) {
        if(filePath[start] == '/') {
            ++start;
            size_new = size - start;
            break;
        }
        --start;
    }

    if(size_new <= FILE_NAME_MAX_LENGTH) {
        /* copy only file name */
        memcpy(fileName, &filePath[start], size_new-1);
        fileName[FILE_NAME_MAX_LENGTH] = '\0';
        *fileNameSize = size_new;
    }
    else
        ret = 1; /* error data too big for buffer */

    return ret;
}


int32_t get_file_data(const char *filePath, int8_t *fileData,  uint32_t *fileSize) {
    uint32_t ret = 0;
    uint32_t size = 0;
    FILE *f = fopen(filePath, "r");

    if(f != NULL) {
        fseek(f, 0, SEEK_END);
        size = ftell(f);

        if(size != 0) {
            fseek(f, 0, 0);

            // read into buffer
            fread(fileData, size, 1, f);
            fileData[size] = '\0';
            *fileSize = size;
        }
        else
            ret = 1; /* error data too big for buffer */

        fclose(f);
    }
    else 
        ret = 2; /* file not found */

    return ret;
}


/******************************************************************************/
/* send file, OD entry 3002 */
/*
CO_SDO_abortCode_t CO_ODF_3003(CO_ODF_arg_t *ODF_arg) {
    CO_OD_file_data_t *odFileData;
    CO_SDO_abortCode_t ret = CO_SDO_AB_NONE;

    odFileData = (CO_OD_file_data_t*) ODF_arg->object;

    APP_LOCK_ODF();

    if(ODF_arg->subIndex == 1) {
        if(ODF_arg->reading) { / * read parameters * /
            ODF_arg->lastSegment = true;
            ODF_arg->dataLength = sizeof(odFileData->fileSize);
            memcpy(ODF_arg->data, &(odFileData->fileSize), ODF_arg->dataLength);
        }
        else 
            return CO_SDO_AB_SUB_UNKNOWN; 
    }
    else if(ODF_arg->subIndex == 2) {
        if(ODF_arg->reading) { / * read parameters * /
            if(odFileData->fileSize == 0) / * error, no data to read * /
                return CO_SDO_AB_NO_DATA; 

            if(ODF_arg->firstSegment) { / * 1st offset * /
                ODF_arg->dataLength = odFileData->fileSize;

                / * check if new data will fit in struct * /
                if(ODF_arg->dataLength > FILE_TRANSFER_MAX_SIZE)
                    return CO_SDO_AB_OUT_OF_MEM; 

                ODF_arg->dataLengthTotal = odFileData->fileSize;
            }

            ODF_arg->lastSegment = true;
            memcpy(ODF_arg->data, odFileData->fileData, ODF_arg->dataLength);
        }
        else { / * write parameters * /
            if(ODF_arg->firstSegment) { / * 1st segment * /
                odFileData->fileSize = ODF_arg->dataLength;

                / * check if new data will fit in struct * /
                if(ODF_arg->dataLength > FILE_TRANSFER_MAX_SIZE)
                    return CO_SDO_AB_OUT_OF_MEM; 
            }

            ODF_arg->lastSegment = true;
            memcpy(odFileData->fileData, ODF_arg->data, ODF_arg->dataLength);
        }
    }
    else 
        return CO_SDO_AB_SUB_UNKNOWN; 

    APP_UNLOCK_ODF();

    return ret;
}

*/
