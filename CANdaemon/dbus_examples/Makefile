CC= gcc

CFLAGS_DBUS = $(shell pkg-config --cflags --libs dbus-1)
CFLAGS_DBUS_GLIB = $(shell pkg-config --cflags --libs dbus-glib-1)
CFLAGS_GIO  = $(shell pkg-config --cflags --libs gio-unix-2.0)
CFLAGS = -g -Wall -Werror

.PHONY: all

all: method_client signal_client server

method_client:
	$(CC) gdbus-example-method-client.c -o method_client $(CFLAGS) $(CFLAGS_DBUS) $(CFLAGS_DBUS_GLIB) $(CFLAGS_GIO)

signal_client:
	$(CC) gdbus-example-signal-client.c -o signal_client $(CFLAGS) $(CFLAGS_DBUS) $(CFLAGS_DBUS_GLIB) $(CFLAGS_GIO)

server:
	$(CC) gdbus-example-server.c -o server $(CFLAGS) $(CFLAGS_DBUS) $(CFLAGS_DBUS_GLIB) $(CFLAGS_GIO)

clean:
	rm -f server signal_client method_client
