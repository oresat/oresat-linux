/**
 * Helper function for dbus.* files.
 */

#ifndef DBUS_HELPERS_H
#define DBUS_HELPERS_H

#include <stdint.h>

/**
 * Initialize thread and create socket for dbus interface.
 *
 * @return 0 on success.
 */
void send_file(char* file_path, 
               uint16_t idx_name, 
               uint8_t subidx_name, 
               uint16_t idx_data, 
               uint8_t subidx_data);


/**
* Initialize thread and create socket for dbus interface.
*
* @return 0 on success.
*/
int read_file(char* file_path, char* file_data);


/**
* Initialize thread and create socket for dbus interface.
*
* @return 0 on success.
*/
int remove_path(char* file_path, char* file_name);


/**
* Initialize thread and create socket for dbus interface.
*
* @return 0 on success.
*/
void send_SDO(uint16_t idx, uint8_t subidx, char* input_data);

#endif
