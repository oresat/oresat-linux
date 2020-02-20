#include <syslog.h>

/*
 * Logs the message in syslog and will output to stderr/stdout, if available.
 * see man vsyslog for prioprity levels
 */
void log_message(int priority, const char *fmt, ...);

// canopen.* needs this
void CO_errExit(char* msg);
