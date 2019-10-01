/*
 *
 */


#ifndef APP_OD_FUNCTIONS_H
#define APP_OD_FUNCTIONS_H

#include "CANopen.h"
#include "CO_driver.h"

#define FILE_TRANSFER_MAX_SIZE 6000
#define FILE_NAME_MAX_LENGTH 50


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
    int8_t      fileName[FILE_NAME_MAX_LENGTH];         /**< From CO_OD_file_transfer_init() */
    int8_t      fileData[FILE_TRANSFER_MAX_SIZE];       /**< From CO_OD_file_transfer_init() */
    uint32_t    fileSize;                               /**< From CO_OD_file_transfer_init() */
    uint16_t    fileNameSize;                           /**< From CO_OD_file_transfer_init() */
} CO_OD_file_data_t;


/**
 * Callbacks for using inside @ref CO_OD_configure() function for OD DOMAIN objects.
 */
CO_SDO_abortCode_t CO_ODF_3002(CO_ODF_arg_t *ODF_arg);


/**
 * Add file to object dictionay.
 *
 * @return 0 on success.
 */
uint32_t APP_ODF_3002(const char* filePath);


/**
* Remove the path from the file path name.
*
* @return 0 on success
*/
int32_t get_file_name(const char *filePath, int8_t *fileName, uint16_t *fileNameSize);


/**
* Read in file and gets its size.
*
* @return 0 on success
*/
int32_t get_file_data(const char *filePath, int8_t *fileData,  uint32_t *fileSize);


#endif
