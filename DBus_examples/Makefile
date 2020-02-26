CC= gcc
CFLAGS= -g -Wall
LDFLAGS= $(shell pkg-config --libs libsystemd)

all: sd-bus-method-example sd-bus-signal-example sd-bus-property-example sd-bus-read-properties-changed-signal

sd-bus-method-example:
	$(CC) $@.c -o $@ $(CFLAGS) $(LDFLAGS)

sd-bus-signal-example:
	$(CC) $@.c -o $@ $(CFLAGS) $(LDFLAGS)

sd-bus-property-example:
	$(CC) $@.c -o $@ $(CFLAGS) $(LDFLAGS)

sd-bus-read-changed-signal:
	$(CC) $@.c -o $@ $(CFLAGS) $(LDFLAGS)

clean:
	rm -f sd-bus-method-example sd-bus-signal-example sd-bus-property-example sd-bus-read-properties-changed-signal

