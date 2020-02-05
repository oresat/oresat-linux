CC= gcc
CFLAGS= $(shell pkg-config --cflags --libs libsystemd) -pthread
DFLAGS= -g -Wall

all: sd-bus-method-example sd-bus-signal-example sd-bus-property-example sd-bus-read-changed-signal

sd-bus-method-example:
	$(CC) sd-bus-method-example.c -o $@ $(CFLAGS) $(DFLAGS)

sd-bus-signal-example:
	$(CC) sd-bus-signal-example.c -o $@ $(CFLAGS) $(DFLAGS)

sd-bus-property-example:
	$(CC) sd-bus-property-example.c -o $@ $(CFLAGS) $(DFLAGS)

sd-bus-read-changed-signal:
	$(CC) sd-bus-read-changed-signal.c -o $@ $(CFLAGS) $(DFLAGS)


clean:
	rm -f sd-bus-method-example sd-bus-signal-example sd-bus-property-example sd-bus-read-changed-signal

