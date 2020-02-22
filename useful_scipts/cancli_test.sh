#!/usr/bin/env bash

if [[ -z $1 ]]; then
	echo "usage: $0 <can device>"
	exit -1
fi

while [[ 1 ]]; do
	cangen $1
	clear
	echo "[$(date)]: exited with: $?"
	sleep 1
done
