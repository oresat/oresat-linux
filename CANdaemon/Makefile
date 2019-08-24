# Makefile for CANdaemon based off of CANopend


#debug on/off
DEBUG ?= 0


##############################################################################


ifeq ($(DEBUG), 1)
DEBUG_FLAGS += -DDEBUG
endif


##############################################################################


CC ?= gcc
CFLAGS_DBUS =	 $(shell pkg-config --cflags --libs dbus-1) 		\
		 $(shell pkg-config --cflags --libs dbus-glib-1) 	\
		 $(shell pkg-config --cflags --libs gio-unix-2.0)
CFLAGS = $(INCLUDE_DIRS)
DEBUG_FLAGS = -Wall -g -Werror
LDFLAGS = -lrt -pthread


STACKDRV_SRC =		./CANopenNode/stack/socketCAN
STACK_SRC =     	./CANopenNode/stack
CANOPENNODE_SRC = 	./CANopenNode
CANDAEMON_SRC = 	./src
OBJ_DICT_SRC =  	./objDict
CANOPENCOMM_SRC =      	./CANopenComm


INCLUDE_DIRS =	-I$(STACKDRV_SRC)	\
		-I$(STACK_SRC)		\
		-I$(CANOPENNODE_SRC)	\
		-I$(CANDAEMON_SRC)	\
		-I$(OBJ_DICT_SRC)	\
		-I$(CANOPENCOMM_SRC)


COC_SOURCES =	$(CANOPENCOMM_SRC)/CANopenCommand.c


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
		$(CANDAEMON_SRC)/CO_master.c        \
		$(CANDAEMON_SRC)/CO_time.c          \
		$(OBJ_DICT_SRC)/CO_OD.c

DBUS_SOURCES = 	$(CANDAEMON_SRC)/dbus.c             \
		$(CANDAEMON_SRC)/dbus_helpers.c


COMM_SOURCES =	$(CANDAEMON_SRC)/CO_command.c       \
		$(CANDAEMON_SRC)/CO_comm_helpers.c


CD_OBJS =	$(CD_SOURCES:%.c=%.o)
COC_OBJS =	$(COC_SOURCES:%.c=%.o) 
DBUS_OBJS =	$(DBUS_SOURCES:%.c=%.o)
COMM_OBJS =	$(COMM_SOURCES:%.c=%.o)
ALL_OBJS = 	$(CD_OBJS) $(COC_OBJS) $(DBUS_OBJS) $(COMM_OBJS)


%.o: %.c
	$(CC) $(CFLAGS) $(CFLAGS_DBUS) $(DEBUG_FLAGS) -c $< -o $@


all: candaemon candaemon-master canopencomm

clean:
	rm -f $(ALL_OBJS) canopencomm candaemon canopend candaemon-master

candaemon: $(CD_OBJS) $(COMM_OBJS) $(DBUS_OBJS)
	$(CC) $(CFLAGS) $(CFLAGS_DBUS) $(DEBUG_FLAGS) $(LDFLAGS) $^ src/main.c -o $@

candaemon-master: $(CD_OBJS) $(COMM_OBJS) $(DBUS_OBJS)
	$(CC) $(CFLAGS) $(CFLAGS_DBUS) $(DEBUG_FLAGS)  $(LDFLAGS) $^ src/main.c -o $@ -DMASTER_NODE

canopend: $(CD_OBJS) $(COMM_OBJS)
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) $(LDFLAGS) $^ src/main.c -o $@ -DMASTER_NODE

canopencomm: $(COC_OBJS)
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) $(LDFLAGS) $^ -o $@

help:
	$(info Make Options:)
	$(info - candaemon: candaemon with no master node options, with dbus on)
	$(info - candaemon-master: candaemon with master node options on and dbus on) 
	$(info - candopend: default canopend and dbus off)
	$(info - canopencomm: default canopencomm)
	$(info - all: make all 4)
	$(info - clean: remove all .o files and binaries)
