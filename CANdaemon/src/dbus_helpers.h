/**
 * Helper function for dbus.* files.
 */

#ifndef DBUS_HELPERS_H
#define DBUS_HELPERS_H


/* Handle the dbus error */
void dbusError(int r, char* err);
void dbusErrorExit(int r, char* err);


#endif
