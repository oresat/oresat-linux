#/bin/bash -e

if [ $# -eq 0 ]; then
    mounted_root_parition=`lsblk | grep "/" | cut -d " " -f 1 | tr -cd "[:alnum:]._-"`
elif [ $1 == "-h" ] || [ $1 == "--help" ]; then
    echo "Will grow the root parition to use all available space"
    echo "Usage: $0 <root parition>"
    echo "If no root parition is given, the mounted root parition will be expaned"
    echo "example: $0 /dev/mmcblk0p2"
    exit 0
else
    mounted_root_parition=`echo $1 | rev | cut -d "/" -f 1 | rev`
fi

growpart_args=`echo $mounted_root_parition | sed -e "s/p/ /g"`
sudo growpart /dev/$growpart_args
sudo resize2fs /dev/$mounted_root_parition
