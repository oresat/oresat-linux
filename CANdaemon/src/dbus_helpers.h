/**
 * Helper function for dbus.* files.
 */

#ifndef DBUS_HELPERS_H
#define DBUS_HELPERS_H

#include <stdint.h>

#define SDO_BUFFER_SIZE     100000000
#define STRING_BUFFER_SIZE ((SDO_BUFFER_SIZE*4)+100)

/* Handle the dbus error */
void dbusError(int r, char* err);
void dbusErrorExit(int r, char* err);


/**
 * Initialize thread and create socket for dbus interface.
 *
 * @return 0 on success.
 */
int OD_add_file(const uint16_t idx, 
                const uint8_t subidx_name, 
                const uint8_t subidx_data,
                const char* file_path);


/**
 * Initialize thread and create socket for dbus interface.
 *
 * @return 0 on success.
 */
int OD_set(const uint16_t idx, 
           const uint8_t subidx,
           const int16_t data);


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


/**
* Gets the i8 value at index and subindex
*
* @return value
*/
int8_t OD_get_i8(const uint16_t idx, const uint8_t subidx);


/**
* Gets the i16 value at index and subindex
*
* @return value
*/
int16_t OD_get_i16(const uint16_t idx, const uint8_t subidx);


/**
* Gets the i32 value at index and subindex
*
* @return value
*/
int32_t OD_get_i32(const uint16_t idx, const uint8_t subidx);


/**
* Gets the i64 value at index and subindex
*
* @return value
*/
int64_t OD_get_i64(const uint16_t idx, const uint8_t subidx);


/**
* Gets the u8 value at index and subindex
*
* @return value
*/
uint8_t OD_get_u8(const uint16_t idx, const uint8_t subidx);


/**
* Gets the u16 value at index and subindex
*
* @return value
*/
uint16_t OD_get_i16(const uint16_t idx, const uint8_t subidx);


/**
* Gets the u32 value at index and subindex
*
* @return value
*/
int32_t OD_get_i32(const uint16_t idx, const uint8_t subidx);


/**
* Gets the u64 value at index and subindex
*
* @return value
*/
uint64_t OD_get_u64(const uint16_t idx, const uint8_t subidx);

#endif
