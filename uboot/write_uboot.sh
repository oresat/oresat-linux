#!/bin/bash

DEVICE=sdb

umount /media/chad/rootfs
dd if=u-boot-dtb.img of=/dev/${DEVICE} count=4 seek=1 bs=384k
dd if=MLO of=/dev/${DEVICE} count=2 seek=1 bs=128k
