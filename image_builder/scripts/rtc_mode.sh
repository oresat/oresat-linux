#!/bin/bash -e

#find a better way to identify the rv3028; For now, we can assume rtc1 = rv3028 but may not always be 
rtc_dev=rtc1
bsm=0x2
lsm=0x2

current_mode=$(hwclock --rtc=/dev/$rtc_dev --param-get=$bsm | grep -oE '0x[0-9a-fA-F]+' | tail -n1)

if [ -z "$current_mode" ]; then
    echo "RTC-RV3028: could not read device mode"
    exit 1
else
    if [ "${current_mode}" == "${lsm}" ]; then
        echo "RTC-RV3028 Level Switching Mode (LSM) Active"
    else
        echo "RTC-RV3028 Level Switching Mode (LSM) Enabled"
        hwclock --rtc=/dev/$rtc_dev --param-set=$bsm=$lsm
    fi
fi
