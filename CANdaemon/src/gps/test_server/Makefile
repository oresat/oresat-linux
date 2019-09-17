CC= gcc
CFLAGS= $(shell pkg-config --cflags --libs libsystemd)
DFLAGS= -g -Wall -Werror


gps-dbus-server:
	$(CC) gps-dbus-server.c -o $@ $(CFLAGS) $(DFLAGS) -pthread


clean:
	rm -f gps-dbus-server

