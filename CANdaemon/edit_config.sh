#!/bin/bash

# ----------------------------------------------------------------------------
# Valid inputs options

# seperate option with spaces
# MUST start and end with spaces

BOARD_OPTIONS=" gps startracker cfc dxwifi template cameraboard "
UPDATER_OPTIONS=" on off "
DEBUG_OPTIONS=" on off "

# ----------------------------------------------------------------------------
# Check if config file exist

FILE=.config
if [ ! -s $FILE ]; then
    echo "run make <board_name>"
    echo "run make help to get the list of boards"
    exit 0
fi 

# ----------------------------------------------------------------------------
# Read in values from config file

BOARD=`grep -r "oresat-board" .config | cut -f 2 -d ':' | sed 's/ //g'`
UPDATER_INTERFACE=`grep -r "updater" .config | cut -f 2 -d ':' | sed 's/ //g'`
DEBUG_FLAG=`grep -r "debug" .config | cut -f 2 -d ':' | sed 's/ //g'`

# ----------------------------------------------------------------------------
# Get inputs

NEW_FILE="# OreSat Linux CANdaemon config file\n"

while : ; do
    echo "Which OreSat board? current is: $BOARD"
    read NEW_OPTION
    if [ -z "$NEW_OPTION" ]; then
        NEW_OPTION=$BOARD
        break
    elif [[ "$BOARD_OPTIONS" == *" $NEW_OPTION "* ]]; then
        break
    fi
done

NEW_FILE=$NEW_FILE"oresat-board:"$NEW_OPTION"\n"

while : ; do
    echo "Enable updater interface? current is: $UPDATER_INTERFACE"
    read NEW_OPTION
    if [ -z "$NEW_OPTION" ]; then
        NEW_OPTION=$UPDATER_INTERFACE
        break
    elif [[ "$UPDATER_OPTIONS" == *" $NEW_OPTION "* ]]; then
        break
    fi
done

NEW_FILE=$NEW_FILE"updater:"$NEW_OPTION"\n"

while : ; do
    echo "Enable updater debug flag? current is: $DEBUG_FLAG"
    read NEW_OPTION
    if [ -z "$NEW_OPTION" ]; then
        NEW_OPTION=$DEBUG_FLAG
        break
    elif [[ "$DEBUG_OPTIONS" == *" $NEW_OPTION "* ]]; then
        break
    fi
done

NEW_FILE=$NEW_FILE"debug:"$NEW_OPTION"\n"

# ----------------------------------------------------------------------------
# Override file

echo -e $NEW_FILE > .config

