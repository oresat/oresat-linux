# Makefile for CANopend.
#
CC= gcc
CFLAGS_DBUS = $(shell pkg-config --cflags --libs dbus-1)
CFLAGS_DBUS_GLIB = $(shell pkg-config --cflags --libs dbus-glib-1)
CFLAGS_GIO  = $(shell pkg-config --cflags --libs gio-unix-2.0)
CFLAGS = -Wall -g $(CFLAGS_DBUS) $(CFLAGS_DBUS_GLIB) $(CFLAGS_GIO)
LDFLAGS = -lrt -pthread


SRC = 		./src
STACK_SRC = 	./src/stack
OBJ_DICT_SRC = 	./src/objDict


LINK_TARGET  =  ./CANdaemon


INCLUDE_DIRS = -I$(STACK_SRC)   		\
               -I$(SRC)				\
               -I$(OBJ_DICT_SRC) 		\


SOURCES =       $(STACK_SRC)/CO_driver.c	\
                $(STACK_SRC)/CO_OD_storage.c	\
                $(STACK_SRC)/CO_Linux_tasks.c	\
                $(STACK_SRC)/crc16-ccitt.c	\
                $(STACK_SRC)/CO_SDO.c		\
                $(STACK_SRC)/CO_Emergency.c	\
                $(STACK_SRC)/CO_NMT_Heartbeat.c	\
                $(STACK_SRC)/CO_SYNC.c		\
                $(STACK_SRC)/CO_PDO.c		\
                $(STACK_SRC)/CO_HBconsumer.c	\
                $(STACK_SRC)/CO_SDOmaster.c	\
                $(STACK_SRC)/CO_trace.c		\
                $(STACK_SRC)/CANopen.c		\
                $(SRC)/dbus.c			\
                $(SRC)/dbus_helpers.c		\
                $(SRC)/CO_comm_helpers.c	\
                $(SRC)/CO_master.c		\
                $(SRC)/CO_time.c		\
                $(SRC)/main.c			\
                $(OBJ_DICT_SRC)/CO_OD.c		\


OBJS = $(SOURCES:%.c=%.o)


.PHONY: all

all: $(LINK_TARGET)

clean:
	rm -f $(OBJS) $(LINK_TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDE_DIRS)

$(LINK_TARGET): $(OBJS)
	$(CC) $(LDFLAGS) $(CFLAGS) $^ -o $@
