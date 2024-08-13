#!/bin/bash

# Creates GPIO-SIM entries that match the device-tree configuration
# described in FILE

# Get the directory of the sourced script
script_dir=$(dirname "${BASH_SOURCE[0]}")

# The device tree file to parse
FILE=$script_dir/../device_trees/oresat-c3-0601.dts

mountpoint /sys/kernel/config > /dev/null || mount -t configfs configfs /sys/kernel/config

# Initialize banks
# 4 banks 32 gpio each
dir=/sys/kernel/config/gpio-sim/basic
for i in {0..3}
do
	mkdir -p $dir/bank$i
	echo 32 > $dir/bank$i/num_lines
done

# Read the device tree file line by line
while IFS= read -r line
do
	# Check if the line contains "gpio-name"
	if [[ $line == *"gpio-name"* ]]; then
	# Extract the gpio-name
	gpio_name=$(echo $line | cut -d'"' -f 2)
	echo "GPIO Name: $gpio_name"

	read -r line
	# Extract the gpiochip number and lane number
	gpiochip_number=$(echo $line | cut -d' ' -f 3 | tr -dc '0-9')
	lane_number=$(echo $line | cut -d' ' -f 4)
	echo "GPIOCHIP Number: $gpiochip_number"
	echo "Lane Number: $lane_number"

	mkdir -p $dir/bank$gpiochip_number/line$lane_number
	echo "$gpio_name" > $dir/bank$gpiochip_number/line$lane_number/name

	# not setting direction becauase gpio-sim does not allow setting an initial direction
	# Only when "hogging" the lane by the kernel it can set as input, output-high, and output-low
	
	# read -r line
	## Extract the direction
	# direction=$(echo $line | tr -d '[:space:]' | sed 's/;$//')
	# echo "Direction: $direction"

	#mkdir -p dir/bank$gpiochip_number/line$lane_number/hog/
	#echo "hog-name-example" > $dir/bank$gpiochip_number/line$lane_number/hog/name
	#echo "$direction" > $dir/bank$gpiochip_number/line$lane_number/hog/direction
	#
	fi

done < "$FILE"

echo 1 > $dir/live
