#!/bin/bash -e

raw=$(sudo cut -c 5-28 /sys/bus/i2c/devices/*/eeprom)
echo "Raw: $raw"

is_oresat_card=1
board_id=$(cut -c 1-8 <<< "$raw")

case "$board_id" in
  "A335OSC3")
    name="C3";;
  "A335OCFC")
    name="CFC";;
  "A335ODWF")
    name="DxWiFi";;
  "A335OGPS")
    name="GPS";;
  "A335OSST")
    name="Star Tracker";;
  "A335PBGL")
    name="PocketBeagle"
    is_oresat_card=0;;
  "A335BNLT")
    name="BeagleBone Black"
    is_oresat_card=0;;
  *)
    name="Unknown"
    is_oresat_card=0;;
esac
echo "Board: $name ($board_id)"


version=$(cut -c 9-12 <<< "$raw")
if [ $is_oresat_card -eq 1 ]; then
  maj_version=$(cut -c 9-10 <<< "$raw" | sed -r "s/0([0-9A-F]*)/\1/")
  min_version=$(cut -c 11-12 <<< "$raw" | sed -r "s/0([0-9A-F]*)/\1/")
  echo "Version: $maj_version.$min_version ($version)"
else
  echo "Version: $version"
fi

week=$(cut -c 13-14 <<< "$raw")
year=$(cut -c 15-16 <<< "$raw")
echo "Manufacture Year: $year"
echo "Manufacture Week: $week"

card_id=$(cut -c 21-24 <<< "$raw" | sed -r "s/0*([0-9A-F]*)/\1/")
echo "Card ID: $card_id"
