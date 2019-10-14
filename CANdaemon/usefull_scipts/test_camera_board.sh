#!/bin/bash

user=`id -u`

if [ $user -ne 0 ]
    then 
        echo "Most be root"
        exit
fi

Camera_PID=`pgrep camera`
CANdaemon_PID=`pgrep candaemon`

if [ -z "$Camera_PID" ]; then
    echo "starting camera_dbus_test_server"
    nohup ../src/app_camera_board/test_process/camera_dbus_test_server &
fi

if [ -z "$CANdaemon_PID" ]; then
    echo "starting candaemon"
    nohup ../src/app_camera_board/candaemon can1 &
fi
