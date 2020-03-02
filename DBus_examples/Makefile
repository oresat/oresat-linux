CC= gcc
CFLAGS= -g -Wall
LDFLAGS= $(shell pkg-config --libs libsystemd) -pthread

EXAMPLES = 	sd-bus-method-client-example 		\
		sd-bus-method-server-example		\
		sd-bus-property-client-example		\
		sd-bus-property-server-example 		\
		sd-bus-read-properties-changed-signal 	\
		sd-bus-signal-client-example		\
		sd-bus-signal-server-example

all: $(EXAMPLES)
	$(CC) $(CFLAGS) $(LDFLAGS) $<.c -o $<

clean:
	rm -f $(EXAMPLES)
