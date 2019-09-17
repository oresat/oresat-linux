# Makefile for CANdaemon based off of CANopend

##############################################################################
# Settings
#0 off
#1 on

GPS_CANDAEMON ?= 1# GPS dbus interface
ST_CANDAEMON ?= 0#Star Tracker dbus interface
UPDATER_CANDAEMON ?= 0#Linux Updater dbus interface
MASTER_NODE ?= 0#Network Manager Node
DEBUG ?= 1# Debug flags


##############################################################################
# General and files


CC ?= gcc
CFLAGS_DBUS= $(shell pkg-config --cflags --libs libsystemd)
CFLAGS = $(INCLUDE_DIRS)
LDFLAGS = -lrt -pthread


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
		$(CANDAEMON_SRC)/CO_time.c          \
		$(OBJDICT_SRC)/CO_OD.c              \
		$(CANDAEMON_SRC)/main.c

DBUS_SOURCES = 	$(CANDAEMON_SRC)/dbus.c             \
		$(CANDAEMON_SRC)/dbus_helpers.c

COMM_SOURCES =	$(CANDAEMON_SRC)/CO_command.c       \
		$(CANDAEMON_SRC)/CO_comm_helpers.c

GPS_SOURCES = 	$(GPS_SRC)/GPS_interface.c

ST_SOURCES = 	$(ST_SRC)/ST_interface.c


##############################################################################
# Handle Settings


ifeq ($(GPS_INTERFACE), 1)
CANDAEMON_SOURCES = $(SOURCES) $(GPS_SOURCES) $(DBUS_SOURCES)
CFLAGS += $(CFLAGS_DBUS) -DGPS_INTERFACE
endif

ifeq ($(ST_INTERFACE), 1)
CANDAEMON_SOURCES = $(SOURCES) $(STARTACKER_SOURCES) $(DBUS_SOURCES)
CFLAGS += $(CFLAGS_DBUS) -DST_INTERFACE
endif

ifeq ($(DIUPDATER_INTERFACE), 1)
SOURCES = $(STARTACKER_SOURCES) $(DBUS_SOURCES) $(CD_SOURCES)
CFLAGS += $(CFLAGS_DBUS) -DUPDATER
endif

ifeq ($(MASTER_NODE), 1)
    CANDAEMON_SOURCES += $(COMM_SOURCES)
    CANOPEND_SOURCES += $(COMM_SOURCES)
    CFLAGS += -DMASTER_NODE
endif

ifeq ($(DEBUG), 1)
    CFLAGS += -Wall -g
endif


##############################################################################
# Compile


CANDAEMON_OBJS = $(CANDAEMON_SOURCES:%.c=%.o)
CANOPEND_OBJS = $(CANOPEND_SOURCES:%.c=%.o)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDE_DIRS) -c $< -o $@

all: candaemon candopend canopencomm

candaemon: $(CANDAEMON_OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ src/main.c -o $@

canopend: $(CANOPEND_OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ src/main.c -o $@ -CANOPEND_ONLY

canopencomm:
	$(CC)  ./CANopenComm/CANopenCommand.c -o $@

clean:
	rm -rf $(CANDAEMON_OBJS) $(CANOPEND_OBJS) candaemon canopend canopencomm

help:
	$(info Make Options:)
	$(info - linux-interface: )
	$(info - canopencomm: default canopencomm)
	$(info - clean: remove all .o files and binaries)
