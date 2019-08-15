# Eample Code
The example code uses GDBus for c and pydbus for python. These example scripts fallow the client server model. The DBus server will output signal with it's data and will process any method calls. The signal-clients will print any signal on the DBus interface. The message-clients will call methods on the server to control the server.

# Depenedices
## For the C version
- gbus, gbus-gLib, glib2, glibc, make
## For the Python version
- gbus, python-pydbus, gbus-gLib, glib2, glibc
