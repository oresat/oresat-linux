# Makefile for CANdaemon based off of CANopend

##############################################################################
# Settings for CANdaemon compile


#System Setting
DFLAGS= -DFILE_TRANSFER_MAX_SIZE=1500000    \
	-DFILE_PATH_MAX_LENGTH=200


##############################################################################
# General and files


CC = gcc
CFLAGS = -Wall $(shell pkg-config --cflags libsystemd) -g
LFLAGS = -lrt -pthread $(shell pkg-config --libs libsystemd)


STACK_SRC =     	../CANopenNode/stack
CANOPENNODE_SRC = 	../CANopenNode
CANOPENCOMM_SRC = 	../CANopenComm
COMMON_SRC = 		../common
APP_SRC =		../src/app_template/source
OBJDICT_SRC =           ./objDict


INCLUDE_DIRS =	-I$(STACK_SRC)		            \
		-I$(CANOPENNODE_SRC)	            \
		-I$(CANOPENCOMM_SRC)                \
		-I$(COMMON_SRC)                     \
		-I$(APP_SRC)                        \
		-I$(OBJDICT_SRC)


SOURCES =	$(COMMON_SRC)/CO_driver.c           \
		$(COMMON_SRC)/CO_OD_storage.c       \
		$(COMMON_SRC)/CO_Linux_tasks.c      \
		$(STACK_SRC)/crc16-ccitt.c          \
		$(STACK_SRC)/CO_SDO.c               \
		$(STACK_SRC)/CO_Emergency.c         \
		$(STACK_SRC)/CO_NMT_Heartbeat.c     \
		$(STACK_SRC)/CO_SYNC.c              \
		$(STACK_SRC)/CO_PDO.c               \
		$(STACK_SRC)/CO_HBconsumer.c        \
		$(STACK_SRC)/CO_SDOmaster.c         \
		$(STACK_SRC)/CO_LSSmaster.c         \
		$(STACK_SRC)/CO_LSSslave.c          \
		$(STACK_SRC)/CO_trace.c             \
		$(CANOPENNODE_SRC)/CANopen.c        \
		$(COMMON_SRC)/app_OD_functions.c    \
		$(COMMON_SRC)/CO_time.c

CANOPENCOMM_SOURCES = $(CANOPENCOMM_SRC)/CO_command.c        \
		      $(CANOPENCOMM_SRC)/CO_master.c         \
		      $(CANOPENCOMM_SRC)/CO_comm_helpers.c

CANOPEND_SOURCES = $(SOURCES)                       \
		   $(CANOPENCOMM_SOURCES)           \
		   $(OBJDICT_SRC)/CO_OD.c           \
		   $(CANOPENCOMM_SRC)/main.c


##############################################################################
# Compile


CANOPEND_OBJS = $(CANOPEND_SOURCES:%.c=%.o)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDE_DIRS) -c $< -o $@ $(DFLAGS)

all: canopend canopencomm

canopend: $(CANOPEND_OBJS)
	$(CC) $(LFLAGS) $(INCLUDE_DIRS) $^ -o $@

canopencomm:
	$(CC) $(CFLAGS) $(DFLAGS) $(CANOPENCOMM_SRC)/CANopenCommand.c -o $@

clean:
	rm -rf $(CANOPEND_OBJS) canopend canopencomm od_storage od_storage_auto

