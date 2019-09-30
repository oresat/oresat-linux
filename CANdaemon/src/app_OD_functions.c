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

    if(ODF_arg->subIndex == 1) {
        if(ODF_arg->reading) { /* read parameters */
            ODF_arg->lastSegment = true;
            ODF_arg->dataLength = sizeof(odFileData->fileSize);
            memcpy(ODF_arg->data, &(odFileData->fileSize), ODF_arg->dataLength);
        }
        else 
            return CO_SDO_AB_SUB_UNKNOWN; 
    }
    else if(ODF_arg->subIndex == 2) {
        if(ODF_arg->reading) { /* read parameters */
            if(odFileData->fileSize == 0) /* error, no data to read */
                return CO_SDO_AB_NO_DATA; 

            if(ODF_arg->firstSegment) { /* 1st offset */
                ODF_arg->dataLength = odFileData->fileSize;

                /* check if new data will fit in struct */
                if(ODF_arg->dataLength > FILE_TRANSFER_MAX_SIZE)
                    return CO_SDO_AB_OUT_OF_MEM; 

                ODF_arg->dataLengthTotal = odFileData->fileSize;
            }

            ODF_arg->lastSegment = true;
            memcpy(ODF_arg->data, odFileData->fileData, ODF_arg->dataLength);
        }
        else { /* write parameters */
            if(ODF_arg->firstSegment) { /* 1st segment */
                odFileData->fileSize = ODF_arg->dataLength;

                /* check if new data will fit in struct */
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


uint32_t APP_ODF_3002(const char *filePath) {
    uint32_t ret = 0;
    uint8_t fileName[FILE_NAME_MAX_LENGTH];

    if(filePath == NULL)
        return 1;
   
    APP_LOCK_ODF();

    odSendFileData.fileSize = get_file_size(filePath);
    ret = remove_path(filePath, fileName);
    ret = read_file(filePath, odSendFileData.fileData, odSendFileData.fileSize);
    //app_writeOD(3002, 1, fileName, strlen((char *)fileName)); // don't send '\0'
    
    APP_UNLOCK_ODF();

    return ret; 
}


uint16_t get_file_size(const char *filePath) {
    size_t fileSize;
    FILE *f = fopen(filePath, "r");

    if(f == NULL) {
        printf("File not found: %s\n", filePath); 
        return 0;
    }

    // get file size
    fseek(f, 0, SEEK_END);
    fileSize = ftell(f);
    fclose(f);

    return fileSize;
}


int32_t remove_path(const char *filePath, uint8_t *fileName) {
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

    if(size_new > FILE_NAME_MAX_LENGTH) 
        return 1;

    /* make the filename of all spaces */
    for(unsigned int i=0; i<FILE_NAME_MAX_LENGTH; ++i) 
        fileName[i] = ' ';

    /* copy only file name */
    memcpy(&fileName[0], &filePath[start], size_new-1);
    fileName[FILE_NAME_MAX_LENGTH] = '\0';

    return ret;
}


int32_t read_file(const char *filePath, uint8_t *fileData,  uint16_t fileSize) {
    FILE *f = fopen(filePath, "r");
    uint32_t ret = 0;

    if(f != NULL) {
        // read into buffer
        fread(fileData, fileSize, 1, f);
        fileData[fileSize] = '\0';

        fclose(f);
    }

    return ret;
}


/******************************************************************************/
