#include "error_logging.h"
#include <stdio.h>
#include <stdbool.h>
#include <syslog.h>
#include <stdarg.h>
#include <errno.h>
#include <stdlib.h>

void logmsg(int priority, const char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	vsyslog(priority, fmt, args);
	vfprintf((priority>4?stdout:stderr), fmt, args);
	va_end(args);
}

/* canopen.* needs this*/
void CO_errExit(char* msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

