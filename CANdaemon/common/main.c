#include "CANopen.h"
#include "CO_Linux_tasks.h"
#include "CO_time.h"
#include "application.h"
#include "updater.h"
#include "systemd_ODF.h"
#include "file_transfer_ODF.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sched.h>
#include <signal.h>
#include <errno.h>
#include <sys/epoll.h>
#include <net/if.h>
#include <linux/reboot.h>
#include <sys/reboot.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <syslog.h>
#include <stdarg.h>


#define TMR_TASK_INTERVAL_NS    (1000000)       /* Interval of taskTmr in nanoseconds */
#define TMR_TASK_OVERFLOW_US    (5000)          /* Overflow detect limit for taskTmr in microseconds */
#define INCREMENT_1MS(var)      (var++)         /* Increment 1ms variable in taskTmr */
#define DEFAULT_PID_FILE        "/run/oresat-candaemon.pid"


/* Global variable increments each millisecond. */
volatile uint16_t           CO_timer1ms = 0U;

/* Mutex is locked, when CAN is not valid (configuration state). May be used
 *  from other threads. RT threads may use CO->CANmodule[0]->CANnormal instead. */
pthread_mutex_t             CO_CAN_VALID_mtx = PTHREAD_MUTEX_INITIALIZER;

/* Other variables and objects */
static int                  rtPriority = -1;    /* Real time priority, configurable by arguments. (-1=RT disabled) */
static int                  mainline_epoll_fd;  /* epoll file descriptor for mainline */
static CO_time_t            CO_time;            /* Object for current time */

/* Realtime thread */
static void*                rt_thread(void* arg);
static pthread_t            rt_thread_id;
static int                  rt_thread_epoll_fd;

static bool                 daemon_flag = false;


/* Signal handler */
volatile sig_atomic_t CO_endProgram = 0;
static void sigHandler(int sig) {
    CO_endProgram = 1;

    // stop dbus services threads
    updater_dbus_end();
    app_dbus_end();
}


/* Helper functions ***********************************************************/
void CO_errExit(char* msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

/* send CANopen generic emergency message */
void CO_error(const uint32_t info) {
    CO_errorReport(CO->em, CO_EM_GENERIC_SOFTWARE_ERROR, CO_EMC_SOFTWARE_INTERNAL, info);
    fprintf(stderr, "canopend generic error: 0x%X\n", info);
}


void logmsg(int priority, const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	vsyslog(priority, fmt, args);
	vfprintf((priority>4?stdout:stderr), fmt, args);
	va_end(args);
}


/******************************************************************************/
/** Mainline and RT thread                                                   **/
/******************************************************************************/
int main (int argc, char *argv[]) {
    int c;
    char *pid_file = DEFAULT_PID_FILE;
    FILE *run_fp = NULL;
    pid_t pid = 0, sid = 0;

    /* Register signal handlers */
    signal(SIGINT, NULL);
    signal(SIGTERM, NULL);

    /* Command line argument processing */
    while ((c = getopt(argc, argv, "d")) != -1) {
        switch (c) {
            case 'd':
                daemon_flag = true;
                break;
            default:
                fprintf(stderr, "Usage: %s [-d]", argv[0]);
                exit(1);
        }
    }

    setlogmask(LOG_UPTO(LOG_NOTICE));
    openlog(argv[0], LOG_PID|LOG_CONS, LOG_DAEMON);

    /* Run as daemon if needed */
    if (daemon_flag) {
        logmsg(LOG_DEBUG, "Starting as daemon...\n");
        /* Fork */
        if ((pid = fork()) < 0) {
            logmsg(LOG_ERR, "Error: Failed to fork!\n");
            exit(EXIT_FAILURE);
        }

        /* Parent process exits */
        if (pid) {
            exit(EXIT_SUCCESS);
        }

        /* Child process continues on */
        /* Log PID */
        if ((run_fp = fopen(pid_file, "w+")) == NULL) {
            logmsg(LOG_ERR, "Error: Unable to open file %s\n", pid_file);
            exit(EXIT_FAILURE);
        }
        fprintf(run_fp, "%d\n", getpid());
        fflush(run_fp);
        fclose(run_fp);

        /* Create new session for process group leader */
        if ((sid = setsid()) < 0) {
            logmsg(LOG_ERR, "Error: Failed to create new session!\n");
            exit(EXIT_FAILURE);
        }

        /* Set default umask and cd to root to avoid blocking filesystems */
        umask(0);
        if (chdir("/") < 0) {
            logmsg(LOG_ERR, "Error: Failed to chdir to root: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }

        /* Redirect std streams to /dev/null */
        if (freopen("/dev/null", "r", stdin) == NULL) {
            logmsg(LOG_ERR, "Error: Failed to redirect streams to /dev/null!\n");
            exit(EXIT_FAILURE);
        }
        if (freopen("/dev/null", "w+", stdout) == NULL) {
            logmsg(LOG_ERR, "Error: Failed to redirect streams to /dev/null!\n");
            exit(EXIT_FAILURE);
        }
        if (freopen("/dev/null", "w+", stderr) == NULL) {
            logmsg(LOG_ERR, "Error: Failed to redirect streams to /dev/null!\n");
            exit(EXIT_FAILURE);
        }
    }

    CO_NMT_reset_cmd_t reset = CO_RESET_NOT;
    int CANdevice0Index = 0;
    bool_t firstRun = true;

    char* CANdevice = "can1";         /* CAN device, configurable by arguments. */
    int nodeId = OD_CANNodeID;      /* Use value from Object Dictionary or set to 1..127 by arguments */

    CANdevice0Index = if_nametoindex(CANdevice);

    if(nodeId < 1 || nodeId > 127) {
        fprintf(stderr, "Wrong node ID (%d)\n", nodeId);
        exit(EXIT_FAILURE);
    }

    if(rtPriority != -1 && (rtPriority < sched_get_priority_min(SCHED_FIFO)
                         || rtPriority > sched_get_priority_max(SCHED_FIFO))) {
        fprintf(stderr, "Wrong RT priority (%d)\n", rtPriority);
        exit(EXIT_FAILURE);
    }

    if(CANdevice0Index == 0) {
        char s[120];
        snprintf(s, 120, "Can't find CAN device \"%s\"", CANdevice);
        CO_errExit(s);
    }


    printf("%s - starting CANopen device with Node ID %d(0x%02X)", argv[0], nodeId, nodeId);


    /* Verify, if OD structures have proper alignment of initial values */
    if(CO_OD_RAM.FirstWord != CO_OD_RAM.LastWord) {
        fprintf(stderr, "Program init - %s - Error in CO_OD_RAM.\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    if(CO_OD_EEPROM.FirstWord != CO_OD_EEPROM.LastWord) {
        fprintf(stderr, "Program init - %s - Error in CO_OD_EEPROM.\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    if(CO_OD_ROM.FirstWord != CO_OD_ROM.LastWord) {
        fprintf(stderr, "Program init - %s - Error in CO_OD_ROM.\n", argv[0]);
        exit(EXIT_FAILURE);
    }


    /* Catch signals SIGINT and SIGTERM */
    if(signal(SIGINT, sigHandler) == SIG_ERR)
        CO_errExit("Program init - SIGINIT handler creation failed");
    if(signal(SIGTERM, sigHandler) == SIG_ERR)
        CO_errExit("Program init - SIGTERM handler creation failed");

    /* increase variable each startup. Variable is automatically stored in non-volatile memory. */
    printf(", count=%u ...\n", ++OD_powerOnCounter);


    while(reset != CO_RESET_APP && reset != CO_RESET_QUIT && CO_endProgram == 0) {
/* CANopen communication reset - initialize CANopen objects *******************/
        CO_ReturnError_t err;

        printf("%s - communication reset ...\n", argv[0]);


        /* Wait other threads (command interface). */
        pthread_mutex_lock(&CO_CAN_VALID_mtx);

        /* Wait rt_thread. */
        if(!firstRun) {
            CO_LOCK_OD();
            CO->CANmodule[0]->CANnormal = false;
            CO_UNLOCK_OD();
        }


        /* Enter CAN configuration. */
        CO_CANsetConfigurationMode(&CANdevice0Index);


        /* initialize CANopen */
        err = CO_init(&CANdevice0Index, nodeId, 0);
        if(err != CO_ERROR_NO) {
            char s[120];
            snprintf(s, 120, "Communication reset - CANopen initialization failed, err=%d", err);
            CO_errExit(s);
        }


        /* Configure callback functions for task control */
        CO_EM_initCallback(CO->em, taskMain_cbSignal);
        CO_SDO_initCallback(CO->SDO[0], taskMain_cbSignal);
        CO_SDOclient_initCallback(CO->SDOclient[0], taskMain_cbSignal);


        /* Initialize time */
        CO_time_init(&CO_time, CO->SDO[0], &OD_time.epochTimeBaseMs, &OD_time.epochTimeOffsetMs, 0x2130);


        /* First time only initialization. */
        if(firstRun) {
            firstRun = false;

            /* Configure epoll for mainline */
            mainline_epoll_fd = epoll_create(4);
            if(mainline_epoll_fd == -1)
                CO_errExit("Program init - epoll_create mainline failed");

            /* Init mainline */
            taskMain_init(mainline_epoll_fd, &OD_performance[ODA_performance_mainCycleMaxTime]);

            /* Configure epoll for rt_thread */
            rt_thread_epoll_fd = epoll_create(2);
            if(rt_thread_epoll_fd == -1)
                CO_errExit("Program init - epoll_create rt_thread failed");

            /* Init taskRT */
            CANrx_taskTmr_init(rt_thread_epoll_fd, TMR_TASK_INTERVAL_NS, &OD_performance[ODA_performance_timerCycleMaxTime]);

            OD_performance[ODA_performance_timerCycleTime] = TMR_TASK_INTERVAL_NS/1000; /* informative */

            /* Create rt_thread */
            if(pthread_create(&rt_thread_id, NULL, rt_thread, NULL) != 0)
                CO_errExit("Program init - rt_thread creation failed");

            /* Set priority for rt_thread */
            if(rtPriority > 0) {
                struct sched_param param;

                param.sched_priority = rtPriority;
                if(pthread_setschedparam(rt_thread_id, SCHED_FIFO, &param) != 0)
                    CO_errExit("Program init - rt_thread set scheduler failed");
            }

            // set up general ODFs
            file_transfer_ODF_setup();
            systemd_ODF_setup();

            // set up dbus services
            updater_dbus_setup();
            app_dbus_setup();
        }


        /* start CAN */
        CO_CANsetNormalMode(CO->CANmodule[0]);
        pthread_mutex_unlock(&CO_CAN_VALID_mtx);


        reset = CO_RESET_NOT;

        printf("%s - running ...\n", argv[0]);


        while(reset == CO_RESET_NOT && CO_endProgram == 0) {
/* loop for normal program execution ******************************************/
            int ready;
            struct epoll_event ev;

            ready = epoll_wait(mainline_epoll_fd, &ev, 1, -1);

            if(ready != 1) {
                if(errno != EINTR) {
                    CO_error(0x11100000L + errno);
                }
            }

            else if(taskMain_process(ev.data.fd, &reset, CO_timer1ms)) {
                /* code was processed in the above function. Additional code process below */
            }

            else {
                /* No file descriptor was processed. */
                CO_error(0x11200000L);
            }
        }
    }


/* program exit ***************************************************************/
    /* join threads */
    CO_endProgram = 1;
    if(pthread_join(rt_thread_id, NULL) != 0) {
        CO_errExit("Program end - pthread_join failed");
    }

    // stop dbus services threads
    updater_dbus_end();
    app_dbus_end();

    /* delete objects from memory */
    CANrx_taskTmr_close();
    taskMain_close();
    CO_delete(&CANdevice0Index);

    printf("%s on %s (nodeId=0x%02X) - finished.\n\n", argv[0], CANdevice, nodeId);

    /* Flush all buffers (and reboot) */
    if(reset == CO_RESET_APP) {
        sync();
        if(reboot(LINUX_REBOOT_CMD_RESTART) != 0) {
            CO_errExit("Program end - reboot failed");
        }
    }

    exit(EXIT_SUCCESS);
}


/* Realtime thread for CAN receive and taskTmr ********************************/
static void* rt_thread(void* arg) {

    /* Endless loop */
    while(CO_endProgram == 0) {
        int ready;
        struct epoll_event ev;

        ready = epoll_wait(rt_thread_epoll_fd, &ev, 1, -1);

        if(ready != 1) {
            if(errno != EINTR) {
                CO_error(0x12100000L + errno);
            }
        }

        else if(CANrx_taskTmr_process(ev.data.fd)) {
            /* code was processed in the above function. Additional code process below */
            INCREMENT_1MS(CO_timer1ms);

            /* Monitor variables with trace objects */
            CO_time_process(&CO_time);
#if CO_NO_TRACE > 0
            for(int i=0; i<OD_traceEnable && i<CO_NO_TRACE; i++) {
                CO_trace_process(CO->trace[i], *CO_time.epochTimeOffsetMs);
            }
#endif

            /* Detect timer large overflow */
            if(OD_performance[ODA_performance_timerCycleMaxTime] > TMR_TASK_OVERFLOW_US && rtPriority > 0 && CO->CANmodule[0]->CANnormal) {
                CO_errorReport(CO->em, CO_EM_ISR_TIMER_OVERFLOW, CO_EMC_SOFTWARE_INTERNAL, 0x22400000L | OD_performance[ODA_performance_timerCycleMaxTime]);
            }
        }

        else {
            /* No file descriptor was processed. */
            CO_error(0x12200000L);
        }
    }

    return NULL;
}
