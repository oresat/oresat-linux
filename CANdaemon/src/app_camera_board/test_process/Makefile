testsrc= camera_dbus_test_server.c testprucam.c camera-i2c.c #qdbmp.c

tester:
	gcc $(testsrc) -lsystemd -o camera_dbus_test_server

install:
	bash -c "sudo cp org.OreSat.CameraBoard.conf /usr/share/dbus-1/session.d/"

