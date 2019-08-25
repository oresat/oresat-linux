CC= gcc
DFLAGS= -g -Wall -Werror


CFLAGS_DBUS= $(shell pkg-config --cflags --libs dbus-1)
CFLAGS_DBUS_GLIB= $(shell pkg-config --cflags --libs dbus-glib-1)
CFLAGS_GIO= $(shell pkg-config --cflags --libs gio-unix-2.0)
CFLAGS_GDBUS= $(CFLAGS_DBUS) $(CFLAGS_DBUS_LIB) $(CFLAGS_GIO)


CFLAGS_SD_BUS= $(shell pkg-config --cflags --libs libsystemd)


all: gdbus-all sd-bus-all


gdbus-all: gdbus-method-client gdbus-signal-client gdbus-server

gdbus-method-client:
	$(CC) gdbus-example-method-client.c -o $@ $(CFLAGS_GDBUS) $(DFLAGS)

gdbus-signal-client:
	$(CC) gdbus-example-signal-client.c -o $@ $(CFLAGS_GDBUS) $(DFLAGS)

gdbus-server:
	$(CC) gdbus-example-server.c -o $@ $(CFLAGS_GDBUS) $(DFLAGS)


sd-bus-all: sd-bus-method-client sd-bus-signal-client sd-bus-server

sd-bus-method-client:
	$(CC) sd-bus-example-method-client.c -o $@ $(CFLAGS_SD_BUS) $(DFLAGS)

sd-bus-signal-client:
	$(CC) sd-bus-example-signal-client.c -o $@ $(CFLAGS_SD_BUS) $(DFLAGS)

sd-bus-server:
	$(CC) sd-bus-example-server.c -o $@ $(CFLAGS_SD_BUS) $(DFLAGS) -pthread


clean:
	rm -f gdbus-server gdbus-signal-client gdbus-method-client sd-bus-server sd-bus-method-client sd-bus-signal-client

