#! /bin/bash

USER=`id -u`

if [ $USER -ne 0 ]
    then 
        echo "Run as root"
        exit
fi

DAEMON_NAME='oresat-linux-updater'
INSTALL_PATH='/usr/bin/'$DAEMON_NAME'/'

if [ ! -f $INSTALL_PATH ]; then
    mkdir DAEMON_NAME
fi

echo 'stoping '$DAEMON_NAME' deamon if it is running'
systemctl stop $DAEMON_NAME.service

rm -rf $INSTALL_PATH

echo 'Copying python scripts to /opt/'$DAEMON_NAME
mkdir -p $INSTALL_PATH
cp ../src/updater_.py $INSTALL_PATH
cp ../src/updater_daemon.py $INSTALL_PATH

echo 'Copying daemon service file to /usr/lib/systemd/system/'
cp ../src/$DAEMON_NAME.service /usr/lib/systemd/system/

echo 'Copying dbus config file to /usr/share/dbus-1/system.d/'
cp ../srcorg.oresat.linux.updater.conf /usr/share/dbus-1/system.d/

sleep 1

echo 'Reloading systemctl'
systemctl daemon-reload

sleep 1

echo 'Starting daemon'
systemctl start $DAEMON_NAME

python3 -m py_compileall $INSTALL_PATH
