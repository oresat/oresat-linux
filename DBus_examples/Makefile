CC= gcc
CFLAGS= -g -Wall
LDFLAGS= $(shell pkg-config --libs libsystemd)

all: sd-bus-method-example sd-bus-signal-example sd-bus-property-example sd-bus-read-changed-signal

sd-bus-method-example:
	$(CC) sd-bus-method-example.c -o $@ $(CFLAGS) $(LDFLAGS)

sd-bus-signal-example:
	$(CC) sd-bus-signal-example.c -o $@ $(CFLAGS) $(LDFLAGS)

sd-bus-property-example:
	$(CC) sd-bus-property-example.c -o $@ $(CFLAGS) $(LDFLAGS)

sd-bus-read-changed-signal:
	$(CC) sd-bus-read-changed-signal.c -o $@ $(CFLAGS) $(LDFLAGS)


clean:
	rm -f sd-bus-method-example sd-bus-signal-example sd-bus-property-example sd-bus-read-changed-signal

