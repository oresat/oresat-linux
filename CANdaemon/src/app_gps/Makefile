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


#System Setting
DFLAGS= -DFILE_TRANSFER_MAX_SIZE=1500000    \
	-DFILE_PATH_MAX_LENGTH=200


##############################################################################
# General and files


CC = gcc
CFLAGS = -Wall $(shell pkg-config --cflags libsystemd)
LFLAGS = -lrt -pthread $(shell pkg-config --libs libsystemd)


STACK_SRC =     	../../CANopenNode/stack
CANOPENNODE_SRC = 	../../CANopenNode
COMMON_SRC = 		../../common
APP_SRC =		./source
OBJDICT_SRC =           ./source/objDict


INCLUDE_DIRS =	-I$(STACK_SRC)		            \
		-I$(CANOPENNODE_SRC)	            \
		-I$(COMMON_SRC)                     \
		-I$(APP_SRC)                        \
		-I$(OBJDICT_SRC)


SOURCES =	$(COMMON_SRC)/main.c                \
		$(COMMON_SRC)/CO_driver.c           \
		$(COMMON_SRC)/app_OD_functions.c    \
		$(COMMON_SRC)/CO_time.c             \
		$(COMMON_SRC)/candaemon.c           \
		$(COMMON_SRC)/app_OD_helpers.c      \
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
		$(OBJDICT_SRC)/CO_OD.c              \
		$(APP_SRC)/application.c


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

CANOPENCOMM_PATH="../../CANopenComm/"

OBJS = $(SOURCES:%.c=%.o)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDE_DIRS) -c $< -o $@ $(DFLAGS)

all: candaemon
	$(MAKE) -C $(CANOPENCOMM_PATH) all
	bash -c "cp $(CANOPENCOMM_PATH)canopend ."
	bash -c "cp $(CANOPENCOMM_PATH)canopencomm ."

candaemon: $(OBJS)
	bash -c "echo - > od_storage"
	bash -c "echo - > od_storage_auto"
	$(CC) $(CFLAGS) $(LFLAGS) $(INCLUDE_DIRS) $^ -o $@

clean:
	rm -rf $(OBJS) candaemon canopend canopencomm od_storage od_storage_auto
	$(MAKE) -C $(CANOPENCOMM_PATH) clean

help:
	$(info Make Options:)
	$(info - candaeamon: canopend with dbus interfaces)
	$(info - canopend: default canopend using candaemon object dictionary)
	$(info - canopencomm: default canopencomm from canopen)
	$(info - clean: remove all .o files and binaries)
