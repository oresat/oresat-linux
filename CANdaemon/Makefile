# Makefile for CANdaemon based off of CANopend

##############################################################################
# Settings for CANdaemon compile


#Which main process
#0 GPS
#1 Star Tracker
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
GPS_SRC =		./src/gps
STARTACKER_SRC =	./src/starTracker


INCLUDE_DIRS =	-I$(STACKDRV_SRC)	\
		-I$(STACK_SRC)		\
		-I$(CANOPENNODE_SRC)	\
		-I$(CANDAEMON_SRC)


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
		$(CANDAEMON_SRC)/CO_time.c          \
		$(CANDAEMON_SRC)/app_OD_functions.c \
		$(CANDAEMON_SRC)/app_OD_helpers.c

COMM_SOURCES =	$(CANDAEMON_SRC)/CO_command.c       \
		$(CANDAEMON_SRC)/CO_master.c        \
		$(CANDAEMON_SRC)/CO_comm_helpers.c

CANOPEND_SOURCES = $(SOURCES) $(COMM_SOURCES)


##############################################################################
# Handle Settings


ifeq ($(MAIN_PROCESS_INTERFACE), 0)
CANDAEMON_SOURCES =	$(SOURCES)                          \
			$(CANDAEMON_SRC)/dbus_helpers.c     \
			$(GPS_SRC)/GPS_interface.c	    \
			$(GPS_SRC)/CO_OD.c
CANOPEND_SOURCES += $(GPS_SRC)/CO_OD.c
INCLUDE_DIRS += -I$(GPS_SRC) 
CFLAGS += -DCANDAEMON -DGPS_INTERFACE

else ifeq ($(MAIN_PROCESS_INTERFACE), 1)
CANDAEMON_SOURCES =	$(SOURCES)                          \
			$(CANDAEMON_SRC)/dbus_helpers.c     \
			$(STARTACKER_SRC)/ST_interface.c    \
			$(STARTACKER_SRC)/CO_OD.c
CANOPEND_SOURCES += $(STARTACKER_SRC)/CO_OD.c
INCLUDE_DIRS += -I$(STARTACKER_SRC)
CFLAGS += -DCANDAEMON -DST_INTERFACE

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
	$(CC) $(CFLAGS) $(LDFLAGS) $(INCLUDE_DIRS) $^ $(CANDAEMON_SRC)/main.c $(CANDAEMON_SRC)/application.c -o $@

canopend: $(CANOPEND_OBJS)
	$(CC) $(LDFLAGS) $(INCLUDE_DIRS) $^ $(CANDAEMON_SRC)/main.c $(CANDAEMON_SRC)/application.c -o $@

canopencomm:
	$(CC) ./CANopenComm/CANopenCommand.c -o $@

clean:
	rm -rf $(CANDAEMON_OBJS) $(CANOPEND_OBJS) candaemon canopend canopencomm

help:
	$(info Make Options:)
	$(info - linux-interface: )
	$(info - canopencomm: default canopencomm using candaemon object dictionary)
	$(info - clean: remove all .o files and binaries)
