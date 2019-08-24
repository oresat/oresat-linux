# Makefile for CANdaemon based off of CANopend

#be a master node yes/no
MASTER_NODE ?= 1
#debug on/off
DEBUG ?= 0


##############################################################################


ifeq ($(MASTER_NODE), 1)
DFLAGS += -DMASTER_NODE
endif


ifeq ($(DEBUG), 1)
DEBUG_FLAGS += -DDEBUG
endif


##############################################################################


CC ?= gcc
CFLAGS_DBUS = $(shell pkg-config --cflags --libs dbus-1)
CFLAGS_DBUS_GLIB = $(shell pkg-config --cflags --libs dbus-glib-1)
CFLAGS_GIO  = $(shell pkg-config --cflags --libs gio-unix-2.0)
CFLAGS = $(INCLUDE_DIRS) $(CFLAGS_DBUS) $(CFLAGS_DBUS_GLIB) $(CFLAGS_GIO)
DEBUG_FLAGS = -Wall -g
LDFLAGS = -lrt -pthread


STACKDRV_SRC =		./CANopenNode/stack/socketCAN
STACK_SRC =     	./CANopenNode/stack
CANOPENNODE_SRC = 	./CANopenNode
CANDAEMON_SRC = 	./src
OBJ_DICT_SRC =  	./objDict
CANOPENCOMM_SRC =      	./canopencomm


INCLUDE_DIRS =	-I$(STACKDRV_SRC)	\
		-I$(STACK_SRC)		\
		-I$(CANOPENNODE_SRC)	\
		-I$(CANDAEMON_SRC)	\
		-I$(OBJ_DICT_SRC)	\
		-I$(CANOPENCOMM_SRC)


COC_SOURCES =	$(APPL_SRC)/CANopenCommand.c


CD_SOURCES =	$(STACKDRV_SRC)/CO_driver.c         \
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
		$(CANDAEMON_SRC)/CO_command.c       \
		$(CANDAEMON_SRC)/CO_comm_helpers.c  \
		$(CANDAEMON_SRC)/CO_master.c        \
		$(CANDAEMON_SRC)/CO_time.c          \
		$(CANDAEMON_SRC)/dbus.c             \
		$(CANDAEMON_SRC)/dbus_helpers.c     \
		$(CANDAEMON_SRC)/main.c             \
		$(OBJ_DICT_SRC)/CO_OD.c


CD_OBJS =	$(CD_SOURCES:%.c=%.o)
COC_OBJS =	$(COC_SOURCES:%.c=%.o)


DFLAGS = -DMASTER_NODE


clean-all:
	rm -f $(CD_OBJS) $(COC_OBJS) canopencomm candaemon candaemon-master

clean-o:
	rm -f $(CD_OBJS) $(COC_OBJS)

candaemon:  $(CD_OBJS) 
	$(CC) $(CFLAGS) $(DFLAGS) $(DEBUG_FLAGS) $(LDFLAGS) $^ -o candaemon

candaemon-master: $(CD_OBJS)
	$(CC) $(CFLAGS) $(DFLAGS) $(DEBUG_FLAGS)  $(LDFLAGS) $^ -o candaemon-master

canopencomm: $(COC_OBJS)
	$(CC) $(CFLAGS) $(DFLAGS) $(DEBUG_FLAGS) $(LDFLAGS) $^ -o canopencomm


%.o: %.c
	$(CC) $(CFLAGS) $(DFLAGS) $(DEBUG_FLAGS) -c $< -o $@
