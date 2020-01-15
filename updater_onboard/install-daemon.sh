#! /bin/bash

user=`id -u`

if [ $user -ne 0 ]
    then 
        echo "Run as root"
        exit
fi

daemon_name='oresat-linux-updater'
install_path='/opt/'$daemon_name'/'

echo 'stoping '$daemon_name' deamon if it is running'
sudo systemctl stop $daemon_name.service

sudo rm -rf $install_path

echo 'Copying python scripts to /opt/'$daemon_name
sudo mkdir -p $install_path
sudo cp updater_daemon.py updater.py $install_path

echo 'Copying daemon service file to /usr/lib/systemd/system/'
sudo cp ./$daemon_name.service /usr/lib/systemd/system/

echo 'Copying dbus config file to /usr/share/dbus-1/system.d/'
sudo cp ./org.OreSat.Updater.conf /usr/share/dbus-1/system.d/

sleep 1

echo 'Reloading systemctl'
sudo systemctl daemon-reload

sleep 1

echo 'Starting daemon'
sudo systemctl start $daemon_name.service

