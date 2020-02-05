#ifndef FILE_TRANSFER_ODF_H
#define FILE_TRANSFER_ODF_H


#include "CANopen.h"
#include "CO_driver.h"


#define FILE_TRANSFER_MAX_SIZE  1000000
#define FILE_PATH_MAX_LENGTH    200
#define SEND_FILE_LIST_SIZE     127 /* must be <= 127 */


/* Struct for ODF 3001 (receiving files). */
typedef struct {
    char        fileName[FILE_PATH_MAX_LENGTH];
    int8_t      fileData[FILE_TRANSFER_MAX_SIZE];
    uint32_t    fileSize;
    bool_t      saveFile;
} received_file_data_t;


/** 
 * Struct for ODF 3003 (sending files). 
 * Used to load and access a file that is in sendableFileList
 */
typedef struct {
    uint8_t     filePointer; 
    char       *fileName;
    char        filePath[FILE_PATH_MAX_LENGTH];
    int8_t      fileData[FILE_TRANSFER_MAX_SIZE];
    uint32_t    fileSize;
    uint32_t    filesAvailable;
    uint32_t    overflow;
    char        fileList[FILE_PATH_MAX_LENGTH][SEND_FILE_LIST_SIZE];
} send_file_data_t;


/**
 * Configure all application OD functions
 */
int file_transfer_ODF_setup(void);


/**
 * Callback for using inside CO_OD_configure() function for writing files 
 * into object dictionary.
 */
CO_SDO_abortCode_t recv_file_ODF(CO_ODF_arg_t *ODF_arg);


/**
 * Callback for using inside CO_OD_configure() function for reading files 
 * from object dictionary.
 */
CO_SDO_abortCode_t send_file_array_ODF(CO_ODF_arg_t *ODF_arg);


/**
 * Add file to object dictionary that can be read by a master CAN node.
 * Ment to be used by other part of the CANdaemon (from the app).
 * An example would be giving the master CAN node a captured image.
 *
 * @return 0 on success.
 */
int app_send_file(const char* filePath);


/**
 * Callback for using inside CO_OD_configure() function for reading files 
 * from object dictionary.
 */
CO_SDO_abortCode_t send_file_ODF(CO_ODF_arg_t *ODF_arg);


#endif
