#! /bin/bash

user=`id -u`

if [ $user -ne 0 ]
    then 
        echo "Run as root"
        exit
fi

echo 'Copying example python script to /usr/bin'
sudo cp ./daemon-example.py /opt/

echo 'Copying example python service file to /usr/lib/systemd/system'
sudo cp ./daemon-example.service /usr/lib/systemd/system/

sudo touch /run/daemon-example.pid

sleep 1

echo 'Reloading systemctl'
sudo systemctl daemon-reload

sleep 1

echo 'Starting daemon'
sudo systemctl start daemon-example.service

