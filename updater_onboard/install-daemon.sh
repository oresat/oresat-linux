#! /bin/bash

user=`id -u`

if [ $user -ne 0 ]
    then 
        echo "Run as root"
        exit
fi

daemon_name='oresat-linux-updater'
install_path='/opt/'$daemon_name'/'

sudo rm -rf $install_path

echo 'Copying python scripts to /opt/'$daemon_name
sudo mkdir -p $install_path
sudo cp src/*.py $install_path

echo 'Copying daemon service file to /etc/systemd/system/'
sudo cp ./src/$daemon_name.service /etc/systemd/system/

echo 'Copying dbus config file to /etc/dbus-1/system.d/'
sudo cp ./src/org.OreSat.Updater.conf /etc/dbus-1/system.d/

sudo touch /run/$daemon_name.pid

sleep 1

echo 'Reloading systemctl'
sudo systemctl daemon-reload

sleep 1

echo 'Starting daemon'
sudo systemctl start $daemon_name.service

