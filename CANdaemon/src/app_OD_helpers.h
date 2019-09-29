/*
 *
 *
 */


#ifndef APP_OD_HELPERS_H
#define APP_OD_HELPERS_H


/**
 * Find the location in the OD array from the OD entryNo value.
 *
 * @param index OD index for OD entry
 *
 * @return array location or 0xFFFF for failure.
 */
uint16_t app_OD_find(uint16_t index);


/**
 * Find the location in the OD array from the OD entryNo value.
 *
 * @param entryNo OD entryNo in CO_OD array
 * @param subIndex OD sub-entryNo for OD entry
 * @param data Buffer for copy data from OD
 * @param length Legth of data buffer
 *
 * @return 0 on success.
 */
uint32_t app_readOD(uint16_t entryNo, uint16_t subIndex, uint8_t *data, uint16_t *length);


/**
 * Find the location in the OD array from the OD entryNo value.
 *
 * @param entryNo OD entryNo in CO_OD array
 * @param subIndex OD sub-entryNo for OD entry
 * @param data Data buffer to copy into OD
 * @param length Legth of data buffer
 *
 * @return 0 on success.
 */
uint32_t app_writeOD(uint16_t entryNo, uint16_t subIndex, uint8_t *data, uint16_t length);


#endif
