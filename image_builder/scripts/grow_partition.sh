#!/bin/bash -e

if [ $# -eq 0 ]; then
    mounted_root_partition=$(df --output=source / | grep /dev)
elif [ "$1" == "-h" ] || [ "$1" == "--help" ]; then
    echo "Will grow the root parition to use all available space"
    echo "Usage: $0 <root parition>"
    echo "If no root parition is given, the mounted root parition will be expaned"
    echo "example: $0 /dev/mmcblk0p2"
    exit 0
else
    mounted_root_partition=$1
fi

mounted_root_partition=$(basename "$mounted_root_partition")
growpart_args=${mounted_root_partition//p/" "}
sudo growpart "/dev/$growpart_args"
sudo resize2fs "/dev/$mounted_root_partition"
