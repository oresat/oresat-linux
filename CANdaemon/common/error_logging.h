
/*
 * Logs the error in syslog and will output to stderr or stdout if available.
 */
void logmsg(int priority, const char *fmt, ...);

/* canopen.* needs this*/
void CO_errExit(char* msg);
