#!/bin/bash

user=`id -u`

if [ $user -ne 0 ]
    then 
        echo "Run as root"
        exit
fi

config-pin P2_09 can
config-pin P2_11 can
ip link set can1 up type can bitrate 1000000
ifconfig can1 up
ifconfig can1 txqueuelen 10000
