# Makefile for CANopend.
#
CC= gcc
CFLAGS_DBUS = $(shell pkg-config --cflags --libs dbus-1)
CFLAGS_DBUS_GLIB = $(shell pkg-config --cflags --libs dbus-glib-1)
CFLAGS_GIO  = $(shell pkg-config --cflags --libs gio-unix-2.0)
CFLAGS = -Wall -Werror -g $(CFLAGS_GIO) $(CFLAGS_DBUS_GLIB) $(CFLAGS_DBUS)
LDFLAGS = -lrt -pthread


SRC =     	./src
OBJ_DICT_SRC =  ./src/objDict


LINK_TARGET  =  ./canopend


INCLUDE_DIRS = -I$(SRC)    \
               -I$(OBJ_DICT_SRC) \


SOURCES =       $(SRC)/CO_driver.c         \
                $(SRC)/CO_OD_storage.c     \
                $(SRC)/CO_Linux_tasks.c    \
                $(SRC)/crc16-ccitt.c       \
                $(SRC)/CO_SDO.c            \
                $(SRC)/CO_Emergency.c      \
                $(SRC)/CO_NMT_Heartbeat.c  \
                $(SRC)/CO_SYNC.c           \
                $(SRC)/CO_PDO.c            \
                $(SRC)/CO_HBconsumer.c     \
                $(SRC)/CO_SDOmaster.c      \
                $(SRC)/CO_trace.c          \
                $(SRC)/CANopen.c           \
                $(SRC)/CO_command.c        \
                $(SRC)/CO_comm_helpers.c   \
                $(SRC)/CO_master.c         \
                $(SRC)/CO_time.c           \
                $(SRC)/main.c              \
                $(OBJ_DICT_SRC)/CO_OD.c    \


OBJS = $(SOURCES:%.c=%.o)


.PHONY: all clean

all: clean $(LINK_TARGET)

clean:
	rm -f $(OBJS) $(LINK_TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDE_DIRS)

$(LINK_TARGET): $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@
