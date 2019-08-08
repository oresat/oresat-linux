CC= gcc
DBUS_FLAGS= -I/usr/include/gio-unix-2.0 -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include -I/usr/lib/libffi-3.2.1/include -pthread -I/usr/include/libmount -I/usr/include/blkid -I/usr/include/dbus-1.0 -I/usr/lib/dbus-1.0/include -lgio-2.0 -lgobject-2.0 -lglib-2.0 -ldbus-1 

.PHONY: all

all: client server

client:
	$(CC) gdbus-example-unix-fd-client.c -o client $(DBUS_FLAGS)

server:
	$(CC) gdbus-example-server.c -o server $(DBUS_FLAGS)

clean:
	rm server client 
