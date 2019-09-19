# Makefile for CANdaemon based off of CANopend

##############################################################################
# Settings for CANdaemon compile


#Which main process
#0 no main process
#1 GPS
#2 Star Tracker
MAIN_PROCESS_INTERFACE ?= 1#


#Optional Interfaces
#0 off
#1 on
UPDATER_INTERFACE ?= 0#Linux Updater dbus interface


#Other
DEBUG ?= 1# Debug flags


##############################################################################
# General and files


CC ?= gcc
CFLAGS = -Wall $(shell pkg-config --cflags libsystemd)
LDFLAGS = -lrt -pthread $(shell pkg-config --libs libsystemd)


STACKDRV_SRC =		./CANopenNode/stack/socketCAN
STACK_SRC =     	./CANopenNode/stack
CANOPENNODE_SRC = 	./CANopenNode
CANDAEMON_SRC = 	./src
OBJDICT_SRC =		./src/objDict
GPS_SRC =		./src/gps
STARTACKER_SRC =	./src/starTracker


INCLUDE_DIRS =	-I$(STACKDRV_SRC)	\
		-I$(STACK_SRC)		\
		-I$(CANOPENNODE_SRC)	\
		-I$(CANDAEMON_SRC)      \
		-I$(OBJDICT_SRC)        \
		-I$(GPS_SRC)            \
		-I$(STARTACKER_SRC)


SOURCES =	$(STACKDRV_SRC)/CO_driver.c         \
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
		$(CANDAEMON_SRC)/CO_master.c        \
		$(CANDAEMON_SRC)/CO_time.c

OBJDICT_SOURCES = $(OBJDICT_SRC)/CO_OD.c

COMM_SOURCES =	$(CANDAEMON_SRC)/CO_command.c       \
		$(CANDAEMON_SRC)/CO_comm_helpers.c

DBUS_SOURCES = 	$(CANDAEMON_SRC)/dbus.c             \
		$(CANDAEMON_SRC)/dbus_helpers.c

GPS_SOURCES = 	$(DBUS_SOURCES)                     \
		$(GPS_SRC)/GPS_interface.c	    \
		$(OBJDICT_SRC)/CO_OD.c

ST_SOURCES = 	$(DBUS_SOURCES)			    \
		$(ST_SRC)/ST_interface.c            \
		$(OBJDICT_SRC)/CO_OD.c

CANDAEMON_SOURCES = $(SOURCES) $(DBUS_SOURCES)
CANOPEND_SOURCES = $(SOURCES) $(COMM_SOURCES) $(OBJDICT_SOURCES)


##############################################################################
# Handle Settings


ifeq ($(MAIN_PROCESS_INTERFACE), 0)
CANDAEMON_SOURCES += $(OBJDICT_SOURCES)
else ifeq ($(MAIN_PROCESS_INTERFACE), 1)
CANDAEMON_SOURCES += $(GPS_SOURCES)
CFLAGS += -DGPS_INTERFACE
else ifeq ($(MAIN_PROCESS_INTERFACE), 2)
CANDAEMON_SOURCES += $(STARTACKER_SOURCES)
CFLAGS += -DST_INTERFACE
endif

ifeq ($(DEBUG), 1)
    CFLAGS += -g
endif


##############################################################################
# Compile


CANDAEMON_OBJS = $(CANDAEMON_SOURCES:%.c=%.o)
CANOPEND_OBJS = $(CANOPEND_SOURCES:%.c=%.o)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDE_DIRS) -c $< -o $@

all: candaemon canopend canopencomm

candaemon: $(CANDAEMON_OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(INCLUDE_DIRS) $^ $(CANDAEMON_SRC)/main.c -o $@

canopend: $(CANOPEND_OBJS)
	$(CC) $(LDFLAGS) $(INCLUDE_DIRS) $^ $(CANDAEMON_SRC)/main.c -o $@ -DCANOPEND_ONLY

canopencomm:
	$(CC) ./CANopenComm/CANopenCommand.c -o $@

clean:
	rm -rf $(CANDAEMON_OBJS) $(CANOPEND_OBJS) candaemon canopend canopencomm

help:
	$(info Make Options:)
	$(info - linux-interface: )
	$(info - canopencomm: default canopencomm)
	$(info - clean: remove all .o files and binaries)
