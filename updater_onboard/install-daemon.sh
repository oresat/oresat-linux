#! /bin/bash

USER=`id -u`

if [ $USER -ne 0 ]
then
    echo "Run as root"
    exit
fi

DAEMON_NAME='oresat-linux-updaterd'
INSTALL_PATH='/opt/'$DAEMON_NAME'/'

if [ ! -f $INSTALL_PATH ]; then
    mkdir -p $INSTALL_PATH
fi

echo 'stoping '$DAEMON_NAME' deamon if it is running'
systemctl stop $DAEMON_NAME.service

rm -rf $INSTALL_PATH

echo 'Copying python scripts to '$INSTALL_PATH'/src'
mkdir -p $INSTALL_PATH
cp -r ./src $INSTALL_PATH

echo 'Copying daemon service file to /usr/lib/systemd/system/'
cp ./$DAEMON_NAME.service /usr/lib/systemd/system/

echo 'Copying dbus config file to /usr/share/dbus-1/system.d/'
cp ./org.OreSat.LinuxUpdater.conf /usr/share/dbus-1/system.d/

sleep 1

echo 'Reloading systemctl'
systemctl daemon-reload

sleep 1

echo 'Starting daemon'
systemctl start $DAEMON_NAME

