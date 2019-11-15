/*
 *
 */


#ifndef APP_OD_FUNCTIONS_H
#define APP_OD_FUNCTIONS_H


#include "CANopen.h"
#include "CO_driver.h"


#ifndef FILE_TRANSFER_MAX_SIZE
    #define FILE_TRANSFER_MAX_SIZE 1000000
#endif

#ifndef FILE_PATH_MAX_LENGTH 
    #define FILE_PATH_MAX_LENGTH 200
#endif

#define SEND_FILE_LIST_SIZE 127 /* must be <= 127 */


/**
 * Configure all application OD functions
 */
void app_ODF_configure(void);


/******************************************************************************/
/* structs */


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


/******************************************************************************/
/* functions */


/**
 * Callback for using inside CO_OD_configure() function for writing files 
 * into object dictionary.
 */
CO_SDO_abortCode_t CO_ODF_3001(CO_ODF_arg_t *ODF_arg);


/**
 * Callback for using inside CO_OD_configure() function for reading files 
 * from object dictionary.
 */
CO_SDO_abortCode_t CO_ODF_3002(CO_ODF_arg_t *ODF_arg);

/**
 * Callback for using inside CO_OD_configure() function for reading files 
 * from object dictionary.
 */
CO_SDO_abortCode_t CO_ODF_3003(CO_ODF_arg_t *ODF_arg);


/**
 * Add file to object dictionay that can be read by a master CAN node.
 *
 * @return 0 on success.
 */
int32_t APP_ODF_3002(const char* filePath);


#endif
