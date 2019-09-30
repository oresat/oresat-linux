/*
 *
 */


#ifndef APP_OD_FUNCTIONS_H
#define APP_OD_FUNCTIONS_H

#include "CANopen.h"
#include "CO_driver.h"

#define FILE_TRANSFER_MAX_SIZE 1000000
#define FILE_NAME_MAX_LENGTH 20


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


/**
 * Add file to object dictionay.
 *
 * @return 0 on success.
 */
uint32_t APP_ODF_3002(const char* filePath);


/**
 * Read in file and gets its size.
 *
 * @return size on success, 0 for failure.
 */
uint16_t get_file_size(const char* filePath);


/**
* Remove the path from the file path name.
*
* @return 0 on success
*/
int32_t remove_path(const char *filePath, uint8_t *fileName);


/**
* Reads in file.
*
* @return 0 on success
*/
int32_t read_file(const char *filePath, uint8_t *fileData,  uint16_t fileSize);


#endif
