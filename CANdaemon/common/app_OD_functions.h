/*
 *
 */


#ifndef APP_OD_FUNCTIONS_H
#define APP_OD_FUNCTIONS_H

#include "CANopen.h"
#include "CO_driver.h"

#ifndef FILE_RECEIVE_FOLDER 
#define FILE_RECEIVE_FOLDER "/tmp/received_files/"
#endif
#ifndef FILE_SEND_FOLDER
#define FILE_SEND_FOLDER "/tmp/send_files/"
#endif
#ifndef FILE_TRANSFER_MAX_SIZE
#define FILE_TRANSFER_MAX_SIZE 6000
#endif
#ifndef FILE_PATH_MAX_LENGTH 
#define FILE_PATH_MAX_LENGTH 100
#endif


/**
 * Configure all application OD functions
 */
void app_ODF_configure(void);


/**
 * Object Dictionary storage object for receiving files.
 *
 * Object is used with CANopen OD objects that are for file transfer.
 */
typedef struct {
    char        fileName[FILE_PATH_MAX_LENGTH];
    uint32_t    fileSize;
    int8_t      fileData[FILE_TRANSFER_MAX_SIZE];
    uint8_t     filesAvalible;
} file_buffer_t;


/**
 * Callbacks for using inside @ref CO_OD_configure() function for OD DOMAIN objects.
 */
CO_SDO_abortCode_t CO_ODF_3001(CO_ODF_arg_t *ODF_arg);
CO_SDO_abortCode_t CO_ODF_3002(CO_ODF_arg_t *ODF_arg);


/**
 * Add file to object dictionay.
 *
 * @return 0 on success.
 */
int32_t APP_ODF_3002(const char* filePath);


/**
* Remove the path from the file path name.
*
* @return 0 on success, and sets fileName.
*/
int32_t get_file_name(const char *filePath, char *fileName);


/**
* Read in file and gets its size.
*
* @return 0 on sucess, and sets fileData and fileSize.
*/
int32_t get_file_data(const char *filePath, int8_t *fileData,  uint32_t *fileSize);


/**
* Checks the directory for any files.
*
* @return 0 on sucess, and sets filePath,
*/
int32_t find_file(char *directory, char *filePath);


#endif
