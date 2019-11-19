#!/bin/bash

value1=00000000
value1=`echo "obase=10; ibase=16; $value1" | bc`

value2=00000001
value2=`echo "obase=10; ibase=16; $value2" | bc`

if [ $value1 -gt $value2 ]; then
    echo "gt than"
else
    echo "error"
fi
