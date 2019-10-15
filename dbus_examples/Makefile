CC= gcc
CFLAGS= $(shell pkg-config --cflags --libs libsystemd)
DFLAGS= -g -Wall -Werror

all: sd-bus-method-example sd-bus-signal-example

sd-bus-method-example:
	$(CC) sd-bus-method-example.c -o $@ $(CFLAGS) $(DFLAGS)

sd-bus-signal-example:
	$(CC) sd-bus-signal-example.c -o $@ $(CFLAGS) $(DFLAGS)


clean:
	rm -f sd-bus-method-example sd-bus-signal-example

