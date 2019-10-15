CC= gcc
CFLAGS= $(shell pkg-config --cflags --libs libsystemd)
DFLAGS= -g -Wall -Werror


sd-bus-method-example:
	$(CC) sd-bus-method-example.c -o $@ $(CFLAGS) $(DFLAGS)


clean:
	rm -f sd-bus-method-example

