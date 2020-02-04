#!/bin/bash

user=`id -u`

if [ $user -ne 0 ]
    then 
        echo "Run as root"
        exit
fi

ip link set can1 up type can bitrate 1000000
