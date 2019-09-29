/*
 *
 */


#ifndef APP_OD_FUNCTIONS_H
#define APP_OD_FUNCTIONS_H

#include "CO_driver.h"
#include "CO_SDO.h"

#define FILE_TRANSFER_MAX_SIZE 1000000


/**
 * Configure all application OD functions
 */
void app_ODF_configure(void);

/**
 * Object Dictionary storage object.
 *
 * Object is used with CANopen OD objects that are for file transfer.
 */
typedef struct {
    uint8_t     fileData[FILE_TRANSFER_MAX_SIZE];       /**< From CO_OD_file_transfer_init() */
    uint32_t    fileSize;                               /**< From CO_OD_file_transfer_init() */
} CO_OD_file_data_t;


/**
 * Callbacks for using inside @ref CO_OD_configure() function for OD DOMAIN objects.
 */
CO_SDO_abortCode_t CO_ODF_3002(CO_ODF_arg_t *ODF_arg);
CO_SDO_abortCode_t CO_ODF_3003(CO_ODF_arg_t *ODF_arg);

uint32_t APP_ODF_3002(uint8_t *data, uint32_t length);


#endif
