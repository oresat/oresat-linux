# Makefile for CANopend.


STACK_SRC =     ./stack
CANOPEND_SRC =  ./src
OBJ_DICT_SRC =  ./objDict


LINK_TARGET  =  ./canopend


INCLUDE_DIRS = -I$(STACK_SRC)    \
               -I$(CANOPEN_SRC)  \
               -I$(CANOPEND_SRC) \
               -I$(OBJ_DICT_SRC) \


SOURCES =       $(STACK_SRC)/CO_driver.c         \
                $(STACK_SRC)/CO_OD_storage.c     \
                $(STACK_SRC)/CO_Linux_tasks.c    \
                $(STACK_SRC)/crc16-ccitt.c          \
                $(STACK_SRC)/CO_SDO.c               \
                $(STACK_SRC)/CO_Emergency.c         \
                $(STACK_SRC)/CO_NMT_Heartbeat.c     \
                $(STACK_SRC)/CO_SYNC.c              \
                $(STACK_SRC)/CO_PDO.c               \
                $(STACK_SRC)/CO_HBconsumer.c        \
                $(STACK_SRC)/CO_SDOmaster.c         \
                $(STACK_SRC)/CO_trace.c             \
                $(CANOPEND_SRC)/CANopen.c           \
                $(CANOPEND_SRC)/CO_command.c        \
                $(CANOPEND_SRC)/CO_comm_helpers.c   \
                $(CANOPEND_SRC)/CO_master.c         \
                $(CANOPEND_SRC)/CO_time.c           \
                $(CANOPEND_SRC)/main.c              \
                $(OBJ_DICT_SRC)/CO_OD.c             \


# canopend is three-threaded application: nonblocking mainline, nonblocking
# rt-thread and blocking command interface thread.
# If there is a need to compile canopend as a single-threaded application, then:
#   - Command interface is not possible, so remove CO_command.c,
#     CO_comm_helpers.c, CO_master.c (and CO_SDOmaster.c) from SOURCES.
#   - Add flag -DCO_SINGLE_THREAD to the CFLAGS.
#   - Remove flag -pthread from LDFLAGS.
# Flag -lrt in LDFLAGS is necessary only with older kernels.


OBJS = $(SOURCES:%.c=%.o)
CC ?= gcc
CFLAGS = -Wall -g $(INCLUDE_DIRS)
LDFLAGS = -lrt -pthread


.PHONY: all clean

all: clean $(LINK_TARGET)

clean:
	rm -f $(OBJS) $(LINK_TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(LINK_TARGET): $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@
