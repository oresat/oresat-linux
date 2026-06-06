#!/bin/bash -e

if [ $# -eq 0 ]; then
  t=($(df --output=source /))
  mounted_root_partition="${t[1]}"
elif [ "$1" == "-h" ] || [ "$1" == "--help" ]; then
  echo "Will grow the root parition to use available space and create a swap partition"
  echo "Usage: $0 <root parition>"
  echo "If no root parition is given, the mounted root parition will be expaned"
  echo "example: $0 /dev/mmcblk0p2"
  exit 0
else
  mounted_root_partition=$1
fi

# Cleanly split the disk and partition number
# For /dev/mmcblk0p2 -> /dev/mmcblk0 and 2
if [[ "$mounted_root_partition" =~ ^(.*p)([0-9]+)$ ]]; then
  disk="${mounted_root_partition%p*}"
  part="${BASH_REMATCH[2]}"
fi

# get disk size in sectors
disk_size_sectors=$(blockdev --getsz "${disk}")
sector_size=$(blockdev --getss "${disk}")

# calculate offset for 512M swap partition at end of disk
swap_size_bytes=$((512 * 1024 * 1024))
swap_size_sectors=$((swap_size_bytes / sector_size))

# assuming that the root partition is directly after the boot partition
start=$(blockdev --getsz "${disk}p1")
offset=$((disk_size_sectors - swap_size_sectors - start))

# resize root to fill blockdev and leave space for swap
echo -e ",+${offset}," | sfdisk -N "${part}" "${disk}"
resize2fs "${mounted_root_partition}"

# create swap partition to fill the remaining space
echo -e ",+,S" | sfdisk --append "${disk}"
