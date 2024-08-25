#!/bin/bash
umount /media/chad/rootfs
dd if=u-boot-dtb.img of=/dev/sdc count=4 seek=1 bs=384k
dd if=MLO of=/dev/sdc count=2 seek=1 bs=128k
