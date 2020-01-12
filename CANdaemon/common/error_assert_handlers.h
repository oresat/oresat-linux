/*
 * Handle the logging for errors/asserts
 */


#include "CO_driver.h"


/*
 * Handle the dbus errors
 */
void dbusError(int r, char* err);
void dbusErrorExit(int r, char* err);


/* Log a error. */
// void error(char* err);


/* Log a error and exit. */
// void errorExit(char* err);


/*
 * Check if the iput value is a negative, if so conver to positve number 
 * for a errno and log the error. Useful for sd-bus errors.
 */
// void assertNegErrno(int r, char* err);


/*
 * Check if the iput value is a negative, if so conver to positve number 
 * for a errno, log the error, and exit. Useful for sd-bus errors.
 */
// void assertNegErrnoExit(int r, char* err);


/* Check if bool is true, otherwise log error. */
// void assert(bool_t r, char* err);


/* Check if bool is true, otherwise log error and exit. */
// void assertExit(bool_t r, char* err);
