/**
 * Command interface for CANdaemon.
 */


#ifndef UPDATER_INTERFACE_H
#define UPDATER_INTERFACE_H


/**
 * Starts signal/property thread.
 *
 * @return 0 on success.
 */
int Updater_interface_init(void);

/**
 * Ends signal/property thread.
 *
 * @return 0 on success.
 */
int Updater_interface_clear(void);


/**
 * Call all method checks for the Updater
 */
int Updater_allMethods();


#endif
