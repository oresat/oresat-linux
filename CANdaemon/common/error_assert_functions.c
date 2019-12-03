/*
 * Handle the logging for errors/asserts
 */


#include "CO_driver.h"
#include "error_asser_functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void error(char* err) {
    fprintf(stderr, "%s\n", err);
    return;
}


void errorExit(char* err) {
    fprintf(stderr, "%s\n", err);
    exit(0);
    return;
}

void assertNegErrno(int r, char* err) {
    if (r < 0)
        fprintf(stderr, "%s %s\n", err, strerror(-r));
    return;
}


void assertNegErrnoExit(int r, char* err) {
    if (r < 0) {
        fprintf(stderr, "%s %s\n", err, strerror(-r));
        exit(0);
    }
    return;
}

void assert(bool_t r, char* err) {
    if (!r)
        fprintf(stderr, "%s\n", err);
    return;
}


void assertExit(bool_t r, char* err) {
    if (!r) {
        fprintf(stderr, "%s\n", err);
        exit(0);
    }
    return;
}

