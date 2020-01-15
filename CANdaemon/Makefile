# Makefile for CANdaemon based off of CANopend


##############################################################################
# Options for CANdaemon, Reading config file

BOARD=$(shell grep -r "oresat-board" .config | cut -f 2 -d ':')

DEBUG=$(shell grep -r "debug" .config | cut -f 2 -d ':')
ifeq ($(DEBUG), on)
    CFLAGS += -g
endif

UPDATER_INTERFACE=$(shell grep -r "updater" .config | cut -f 2 -d ':')
ifeq ($(UPDATER_INTERFACE), on)
    DFLAGS += -DUPDATER
endif

#System Setting
DFLAGS= -DFILE_TRANSFER_MAX_SIZE=1500000    \
	-DFILE_PATH_MAX_LENGTH=200


##############################################################################
# General and files

CC = gcc
CFLAGS = -Wall $(shell pkg-config --cflags libsystemd)
LFLAGS = -lrt -pthread $(shell pkg-config --libs libsystemd)

STACK_SRC =     	./CANopenNode/stack
CANOPENNODE_SRC = 	./CANopenNode
COMMON_SRC = 		./common
APP_SRC =		./boards/$(BOARD)
OBJDICT_SRC =           ./boards/$(BOARD)/objDict

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
		$(COMMON_SRC)/updater.c      	    \
		$(COMMON_SRC)/error_assert_handlers.c   \
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
# Compile

OBJS = $(SOURCES:%.c=%.o)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDE_DIRS) -c $< -o $@ $(DFLAGS)

candaemon: $(OBJS)
	@if [ ! -f .config ]; then \
		exit 1; \
	fi

	bash -c "echo - > od_storage"
	bash -c "echo - > od_storage_auto"
	$(CC) $(CFLAGS) $(LFLAGS) $(INCLUDE_DIRS) $^ -o $@

config:
	bash ./edit_config.sh

clean:
	rm -rf $(OBJS) candaemon od_storage od_storage_auto

# boards

cameraboard:
	bash -c "cp ./boards/$@/config .config"

gps:
	bash -c "cp ./boards/$@/config .config"

startracker:
	bash -c "cp ./boards/$@/config .config"

template:
	bash -c "cp ./boards/$@/config .config"

