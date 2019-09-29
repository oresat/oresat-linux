/*
 *
 */


#include "string.h"
#include "CANopen.h"
#include "CO_driver.h"
#include "app_OD_functions.h"


/* Static Variables */
static CO_OD_file_data_t    odSendFileData;     /* Pointer to file data for DOMAIN based OD entry */
static CO_OD_file_data_t    odReceiveFileData;  /* Pointer to file data for DOMAIN based OD entry */


void app_ODF_configure(void){
    /* Add data structs to SDO Object Dictionary */
    CO_OD_configure(CO->SDO[0], 0x3002, CO_ODF_3002, (void*)&odSendFileData, 0, 0U);
    CO_OD_configure(CO->SDO[0], 0x3003, CO_ODF_3003, (void*)&odReceiveFileData, 0, 0U);

}


/******************************************************************************/
/* file transfer */

static CO_SDO_abortCode_t file_transfer(CO_ODF_arg_t *ODF_arg) {
    CO_OD_file_data_t *odFileData;
    CO_SDO_abortCode_t ret = CO_SDO_AB_NONE;

    odFileData = (CO_OD_file_data_t*) ODF_arg->object;

    if(ODF_arg->subIndex != 2)
        return  CO_SDO_AB_SUB_UNKNOWN; 

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

    return ret;
}

CO_SDO_abortCode_t CO_ODF_3002(CO_ODF_arg_t *ODF_arg) {
    return file_transfer(ODF_arg);
}

CO_SDO_abortCode_t CO_ODF_3003(CO_ODF_arg_t *ODF_arg) {
    return file_transfer(ODF_arg);
}

uint32_t APP_ODF_3002(uint8_t *data, uint32_t length) {
    return 0;
}

/******************************************************************************/
/* file transfer */
