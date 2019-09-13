# Makefile for CANdaemon based off of CANopend


#debug on/off
DEBUG ?= 1


##############################################################################


CC ?= gcc
CFLAGS_DBUS= $(shell pkg-config --cflags --libs libsystemd)
CFLAGS = $(INCLUDE_DIRS)
LDFLAGS = -lrt -pthread

ifeq ($(DEBUG), 1)
DEBUG_FLAGS += -Wall -g -Werror -DDEBUG
endif


STACKDRV_SRC =		./CANopenNode/stack/socketCAN
STACK_SRC =     	./CANopenNode/stack
CANOPENNODE_SRC = 	./CANopenNode
INTERFACE_SRC = 	./src
OBJ_DICT_SRC =  	./objDict
CANOPENCOMM_SRC =      	./CANopenComm
GPS_SRC =		./src/gps


INCLUDE_DIRS =	-I$(STACKDRV_SRC)	\
		-I$(STACK_SRC)		\
		-I$(CANOPENNODE_SRC)	\
		-I$(INTERFACE_SRC)	\
		-I$(OBJ_DICT_SRC)	\
		-I$(CANOPENCOMM_SRC)	\
		-I$(GPS_SRC)


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
		$(INTERFACE_SRC)/CO_master.c        \
		$(INTERFACE_SRC)/CO_time.c          \
		$(OBJ_DICT_SRC)/CO_OD.c

DBUS_SOURCES = 	$(INTERFACE_SRC)/dbus_helpers.c


COMM_SOURCES =	$(INTERFACE_SRC)/CO_command.c       \
		$(INTERFACE_SRC)/CO_comm_helpers.c


CD_OBJS =	$(CD_SOURCES:%.c=%.o)
COC_OBJS =	$(COC_SOURCES:%.c=%.o) 
DBUS_OBJS =	$(DBUS_SOURCES:%.c=%.o)
COMM_OBJS =	$(COMM_SOURCES:%.c=%.o)
ALL_OBJS = 	$(CD_OBJS) $(COC_OBJS) $(DBUS_OBJS) $(COMM_OBJS)


%.o: %.c
	$(CC) $(CFLAGS) $(CFLAGS_DBUS) $(DEBUG_FLAGS) $(INCLUDE_DIRS) -c $< -o $@


all: gps-linux-interface gps-linux-interface-master canopend canopencomm

clean:
	rm -f $(ALL_OBJS) gps-linux-interface gps-linux-interface-master canopend canopencomm

gps-linux-interface: $(CD_OBJS) $(DBUS_OBJS)
	$(CC) $(CFLAGS) $(CFLAGS_DBUS) $(DEBUG_FLAGS) $(LDFLAGS) $^ $(GPS_SRC)/dbus.c src/main.c -o $@ -DGPS

gps-linux-interface-master: $(CD_OBJS) $(COMM_OBJS) $(DBUS_OBJS)
	$(CC) $(CFLAGS) $(CFLAGS_DBUS) $(DEBUG_FLAGS)  $(LDFLAGS) $^ $(GPS_SRC)/dbus.c src/main.c -o $@ -DGPS -DMASTER_NODE

canopend: $(CD_OBJS) $(COMM_OBJS)
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) $(LDFLAGS) $^ src/main.c -o $@ -DCANOPEND_ONLY

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
