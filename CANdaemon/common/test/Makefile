
CC = gcc
CFLAGS = -Wall $(shell pkg-config --cflags libsystemd)
LFLAGS = -lrt -pthread $(shell pkg-config --libs libsystemd)

run:
	$(CC) $(CFLAGS) $(LFLAGS) systemd_ODF.c

