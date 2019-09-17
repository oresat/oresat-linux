/**
 * Command interface for CANdaemon.
 */


#ifndef CD_DBUS_H
#define CD_DBUS_H


/**
 * Initialize all dbus threads. One thread for handling the object dictionary 
 * to DBus methods. Then a thread for each process to handle reading signals 
 * and properties into the CAN object dictionary.
 *
 * @return 0 on success.
 */
int dbus_init(void);


/**
 * Terminates all dbus threads.
 *
 * @return 0 on success.
 */
int dbus_clear(void);


#endif
