/**
 * Helper function for dbus.* files.
 */

#ifndef DBUS_HELPERS_H
#define DBUS_HELPERS_H

#include <stdint.h>

#define SDO_BUFFER_SIZE     100000000
#define STRING_BUFFER_SIZE ((SDO_BUFFER_SIZE*4)+100)


/**
 * Initialize thread and create socket for dbus interface.
 *
 * @return 0 on success.
 */
int send_file(const char* file_path, 
              const uint16_t idx, 
              const uint8_t subidx_name, 
              const uint8_t subidx_data);

/**
 * Initialize thread and create socket for dbus interface.
 *
 * @return 0 on success.
 */
unsigned int get_file_size(const char* file_path);

/**
* Initialize thread and create socket for dbus interface.
*
* @return 0 on success.
*/
char* read_file(const char* file_path, unsigned int file_size);


/**
* Initialize thread and create socket for dbus interface.
*
* @return 0 on success.
*/
char* remove_path(const char* file_path);


/**
* Initialize thread and create socket for dbus interface.
*
* @return 0 on success.
*/
void send_SDO(uint16_t idx, uint8_t subidx, char* input_data, uint32_t len);


#endif
