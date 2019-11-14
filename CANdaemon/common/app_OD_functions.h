/*
 *
 */


#ifndef APP_OD_FUNCTIONS_H
#define APP_OD_FUNCTIONS_H


#include "CANopen.h"
#include "CO_driver.h"


/**
 * Configure all application OD functions
 */
void app_ODF_configure(void);


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
 * Add file to object dictionay that can be read by a master CAN node.
 *
 * @return 0 on success.
 */
int32_t APP_ODF_3002(const char* filePath);


#endif
