#/bin/bash -e

raw=`sudo cat /sys/bus/i2c/devices/*/eeprom | cut -c 5-28`
echo "Raw: $raw"

is_oresat_card=1
board_id=`echo $raw | cut -c 1-8`
if [ $board_id = "A335OSC3" ]; then
  name="C3"
elif [ $board_id = "A335OCFC" ]; then
  name="CFC"
elif [ $board_id = "A335ODWF" ]; then
  name="DxWiFi"
elif [ $board_id = "A335OGPS" ]; then
  name="GPS"
elif [ $board_id = "A335OSST" ]; then
  name="Star Tracker"
elif [ $board_id = "A335PBGL" ]; then
  name="PocketBeagle"
  is_oresat_card=0
elif [ $board_id = "A335BNLT" ]; then
  name="BeagleBone Black"
  is_oresat_card=0
else
  name="Unknown"
  is_oresat_card=0
fi
echo "Board: $name ($board_id)"


version=`echo $raw | cut -c 9-12`
if [ $is_oresat_card -eq 1 ]; then
  maj_version=`echo $raw | cut -c 9-10 | sed -r "s/0([0-9A-F]*)/\1/"`
  min_version=`echo $raw | cut -c 11-12 | sed -r "s/0([0-9A-F]*)/\1/"`
  echo "Version: $maj_version.$min_version ($version)"
else
  echo "Version: $version"
fi

week=`echo $raw | cut -c 13-14`
year=`echo $raw | cut -c 15-16`
echo "Manufacture Year: $year"
echo "Manufacture Week: $week"

card_id=`echo $raw | cut -c 21-24 | sed -r "s/0*([0-9A-F]*)/\1/"`
echo "Card ID: $card_id"
