# Makefile for CANdaemon based off of CANopend

##############################################################################
# Settings for CANdaemon compile


#Optional Interfaces
#0 off
#1 on
UPDATER_INTERFACE ?= 0#Linux Updater dbus interface


#Debug flags
#0 off
#1 on
DEBUG ?= 1# Debug flags


##############################################################################
# General and files


CC = gcc
CFLAGS = -Wall $(shell pkg-config --cflags libsystemd)
LDFLAGS = -lrt -pthread $(shell pkg-config --libs libsystemd)


STACKDRV_SRC =		../../CANopenNode/stack/socketCAN
STACK_SRC =     	../../CANopenNode/stack
CANOPENNODE_SRC = 	../../CANopenNode
CANOPENCOMM_SRC = 	../../CANopenComm
COMMON_SRC = 		../../common
APP_SRC =		./source
OBJDICT_SRC =           ./source/ObjDict


INCLUDE_DIRS =	-I$(STACKDRV_SRC)	            \
		-I$(STACK_SRC)		            \
		-I$(CANOPENNODE_SRC)	            \
		-I$(CANOPENCOMM_SRC)                \
		-I$(COMMON_SRC)                     \
		-I$(APP_SRC)                        \
		-I$(OBJDICT_SRC)


SOURCES =	$(COMMON_SRC)/CO_driver.c           \
		$(STACKDRV_SRC)/CO_OD_storage.c     \
		$(STACKDRV_SRC)/CO_Linux_tasks.c    \
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
		$(COMMON_SRC)/CO_time.c             \
		$(COMMON_SRC)/app_OD_functions.c    \
		$(COMMON_SRC)/app_OD_helpers.c

CANOPENCOMM_SOURCES = $(CANOPENCOMM_SRC)/CO_command.c        \
		      $(CANOPENCOMM_SRC)/CO_master.c         \
		      $(CANOPENCOMM_SRC)/CO_comm_helpers.c

CANOPEND_SOURCES = $(SOURCES)                       \
		   $(CANOPENCOMM_SOURCES)           \
		   $(OBJDICT_SRC)/CO_OD.c           \
		   $(CANOPENCOMM_SRC)/main.c

CANDAEMON_SOURCES = $(SOURCES)                      \
		    $(APP_SRC)/application.c        \
		    $(OBJDICT_SRC)/CO_OD.c          \
		    $(COMMON_SRC)/main.c


##############################################################################
# Options


ifeq ($(DEBUG), 1)
    CFLAGS += -g
endif

ifeq ($(UPDATER_INTERFACE), 1)
    CFLAGS += -DUPDATER
endif


##############################################################################
# Compile


CANDAEMON_OBJS = $(CANDAEMON_SOURCES:%.c=%.o)
CANOPEND_OBJS = $(CANOPEND_SOURCES:%.c=%.o)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDE_DIRS) -c $< -o $@

all: candaemon canopend canopencomm

candaemon: $(CANDAEMON_OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(INCLUDE_DIRS) $^ -o $@

canopend: $(CANOPEND_OBJS)
	$(CC) $(LDFLAGS) $(INCLUDE_DIRS) $^ -o $@

canopencomm:
	$(CC) $(CANOPENCOMM_SRC)/CANopenCommand.c -o $@

clean:
	rm -rf $(CANDAEMON_OBJS) $(CANOPEND_OBJS) candaemon canopend canopencomm

help:
	$(info Make Options:)
	$(info - candaeamon: canopend with dbus interfaces)
	$(info - canopend: default canopend using candaemon object dictionary)
	$(info - canopencomm: default canopencomm from canopen)
	$(info - clean: remove all .o files and binaries)
