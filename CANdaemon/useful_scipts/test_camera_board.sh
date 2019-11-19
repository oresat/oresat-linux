#!/bin/bash


user=`id -u`

if [ $user -ne 0 ]
    then 
        echo "Most be root"
        exit
fi


Camera_PID=`pgrep camera`

if [ -z "$Camera_PID" ]; then
    echo "starting camera_dbus_test_server"
else 
    echo "killing old camera_dbus_test_server"
    kill $Camera_PID
    echo "starting new camera_dbus_test_server"
fi

nohup ../src/app_camera_board/test_process/camera_dbus_test_server &


CANdaemon_PID=`pgrep candaemon`

if [ -z "$CANdaemon_PID" ]; then
    echo "starting candaemon"
else 
    echo "killing old candamon"
    kill $Camera_PID
    echo "starting new candaemon"
fi

nohup ../src/app_camera_board/candaemon can1 &
