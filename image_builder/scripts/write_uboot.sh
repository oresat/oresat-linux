#!/bin/bash

DEVICE=$1

dd if=u-boot-dtb.img of=${DEVICE} count=4 seek=1 bs=384k
dd if=MLO of=${DEVICE} count=2 seek=1 bs=128k
