/**
 * Command interface for CANdaemon.
 */


#ifndef ST_INTERFACE_H
#define ST_INTERFACE_H


/**
 * Starts signal/property thread.
 *
 * @return 0 on success.
 */
int ST_interface_init(void);

/**
 * Ends signal/property thread.
 *
 * @return 0 on success.
 */
int ST_interface_clear(void);


/**
 * Call all method checks for the ST
 */
int ST_allMethods();


#endif
