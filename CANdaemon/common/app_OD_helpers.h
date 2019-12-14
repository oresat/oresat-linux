/*
 *
 *
 */


#ifndef APP_OD_HELPERS_H
#define APP_OD_HELPERS_H


/**
 * Find the location in the OD array from the OD index value.
 *
 * @param index OD index for OD entry
 *
 * @return array location or 0xFFFF for failure.
 */
uint16_t app_OD_find(uint16_t index);


/**
 * Find the location in the OD array from the OD index value.
 *
 * @param index OD index for OD entry
 * @param subIndex OD sub-index for OD entry
 * @param data Buffer for copy data from OD
 * @param length Length of data buffer in bytes
 *
 * @return 0 on success.
 */
uint32_t app_readOD(uint16_t index, uint16_t subIndex, void *data, uint16_t *length);


/**
 * Find the location in the OD array from the OD index value.
 *
 * @param index OD index for OD entry
 * @param subIndex OD sub-index for OD entry
 * @param data Data buffer to copy into OD
 * @param length Length of data buffer in bytes
 *
 * @return 0 on success.
 */
uint32_t app_writeOD(uint16_t index, uint16_t subIndex, void *data, uint16_t length);


#endif
