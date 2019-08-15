#!/bin/bash

user=`id -u`

if [ $user -ne 0 ]
  then echo "Run as root"
  exit
fi

killall -q slcand
slcand -o -c -f -s6 /dev/serial/by-id/usb-CANtact_CANtact_dev_00000000001A-if00 can0
ip link set can0 up
