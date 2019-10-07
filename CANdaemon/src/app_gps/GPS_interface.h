/**
 * Command interface for CANdaemon.
 */


#ifndef GPS_INTERFACE_H
#define GPS_INTERFACE_H


/**
 * Starts signal/property thread.
 *
 * @return 0 on success.
 */
int GPS_interface_init(void);

/**
 * Ends signal/property thread.
 *
 * @return 0 on success.
 */
int GPS_interface_clear(void);


/**
 * Call all method checks for the GPS
 */
int GPS_allMethods();


#endif
