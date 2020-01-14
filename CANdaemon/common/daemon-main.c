#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <signal.h>
#include <syslog.h>
#include <pthread.h>

#define DEFAULT_PID_FILE "/var/run/oresat-candaemon.pid"

int main(int argc, char *argv[]) {
    int ret = 0;

    char *pid_file = DEFAULT_PID_FILE;
    FILE *run_fp = NULL;
    pid_t pid = 0 , sid = 0;

    //Fork off the parent process
    pid = fork();
    if (pid < 0) {
        exit(EXIT_FAILURE);
    }
    //If good PID, then exit the parent process
    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }
    
    //Logs PID
    if(( run_fp = fopen( pid_file , "w+" )) == NULL){
        fprintf(stderr, "ERROR: Failed to create a new session!\n");
        exit(EXIT_FAILURE);
    }

    fprintf(run_fp, "%d\n" , getpid());
    fflush(run_fp);
    fclose(run_fp);

    //Change the file mode mask
    umask(0);

    //Create a new SID for the child process
    sid = setsid();
    if (sid < 0) {
        exit(EXIT_FAILURE);
    }

    //Change the current working directory
    if ((chdir("/")) < 0) {
        exit(EXIT_FAILURE);
    }

    //Close out the standard file descriptors
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    candaemon("can");

    return ret;
}
