CC= gcc
CFLAGS= $(shell pkg-config --cflags --libs libsystemd)
DFLAGS= -g -Wall -Werror


all: sd-bus-method-client sd-bus-signal-client sd-bus-server

sd-bus-method-client:
	$(CC) sd-bus-example-method-client.c -o $@ $(CFLAGS) $(DFLAGS)

sd-bus-signal-client:
	$(CC) sd-bus-example-signal-client.c -o $@ $(CFLAGS) $(DFLAGS)

sd-bus-server:
	$(CC) sd-bus-example-server.c -o $@ $(CFLAGS) $(DFLAGS) -pthread


clean:
	rm -f sd-bus-server sd-bus-method-client sd-bus-signal-client

