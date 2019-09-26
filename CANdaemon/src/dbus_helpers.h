/**
 * Helper function for dbus.* files.
 */

#ifndef DBUS_HELPERS_H
#define DBUS_HELPERS_H


#include "CO_driver.h"
#include "CO_SDO.h"


/* Maximum size of Object Dictionary variable transmitted via SDO. */
#ifndef CO_COMMAND_SDO_BUFFER_SIZE
#define CO_COMMAND_SDO_BUFFER_SIZE     1000000
#endif



/* Handle the dbus error */
void dbusError(int r, char* err);
void dbusErrorExit(int r, char* err);


/**
 * Add file to object dictionay.
 *
 * @return 0 on success.
 */
int OD_add_file(const uint16_t idx, 
                const uint8_t subidx_name, 
                const uint8_t subidx_data,
                const char* file_path);



/**
 * Read in file and gets its size.
 *
 * @return size on success, 0 for failure.
 */
unsigned int get_file_size(const char* file_path);

/**
* Reads in file.
*
* @return file_data
*/
char* read_file(const char* file_path, unsigned int file_size);


/**
* Remove the path from the file path name.
*
* @return file name without path.
*/
char* remove_path(const char* file_path);


/**
* Update thes data in object dictiony by use of CANopenNode functions. The nodeID
* is always itself, so no actual sdo message make it onto the CANbus.
*
* @return 0 on success.
*/
int OD_setData(uint16_t idx, uint8_t subidx, void* input_data, const uint32_t dataTxLen);


/**
* Gets value at the index/subindex. Only use on non array data types; ints, floats, bools.
*
* @return 0 on success.
*/
int OD_getNonArrayData(const uint16_t idx, const uint8_t subidx, void* dataOut, const int dataLength);


/**
* Gets value at the index/subindex. Only use on array data types; strings and domains.
*
* @return 0 on success.
*/
int OD_getArrayData(const uint16_t idx, const uint8_t subidx, char* dataOut, int32_t* dataLength);


/**
 * Object Dictionary storage object.
 *
 * Object is used with CANopen OD objects that are for file transfer.
 */
typedef struct {
    uint8_t    *fileData;       /**< From CO_OD_file_transfer_init() */
    uint32_t    fileSize;       /**< From CO_OD_file_transfer_init() */
} CO_OD_file_data_t;


/**
 * Callbacks for using inside @ref CO_OD_configure() function for OD DOMAIN objects.
 */
CO_SDO_abortCode_t file_transfer(CO_ODF_arg_t *ODF_arg);


/**
 * Constructor for data file struct.
 */
CO_ReturnError_t CO_OD_file_transfer_init(CO_OD_file_data_t *odFileData);


/**
 * Deconstructor for data file struct.
 */
void CO_OD_file_transfer_close(CO_OD_file_data_t *odFileData);


#endif
