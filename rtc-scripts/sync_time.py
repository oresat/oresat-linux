#!/usr/bin/env python3
"""
OreSat C3 Minimal Time Synchronization Script
Queries an NTP server over the USB network interface, updates the system clock,
and synchronizes the hardware Real-Time Clock (RTC) via direct ioctl system calls.
"""

import socket
import struct
import time
import subprocess
import fcntl
import sys
from typing import Optional

# Configuration Constants
NTP_SERVER: str = "10.42.0.1"
NTP_PORT: int = 123
TIME1900: int = 2208988800  # Epoch offset between 1900 (NTP epoch) and 1970 (Unix epoch)
RTC_SET_TIME: int = 0x4024700a  # ioctl request code for setting RTC time (struct rtc_time)

def sync_ntp_time(rtc_device: str) -> None:
    """
    Connects to the host NTP server, retrieves the current UTC time,
    updates the Linux system clock, and writes the time to the specified hardware RTC device.
    
    :param rtc_device: Path to the RTC character device node (e.g., '/dev/rtc1' or '/dev/rtc0')
    """
    print(f"Connecting to NTP server at {NTP_SERVER}:{NTP_PORT}...")
    
    # Create a UDP socket with a 3-second timeout to prevent hanging on network drops
    client: socket.socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    client.settimeout(3.0)
    
    try:
        # Construct a standard 48-byte NTP client request packet (Mode 3, Version 3)
        data: bytes = b"\x1b" + 47 * b"\0"
        client.sendto(data, (NTP_SERVER, NTP_PORT))
        
        # Wait for the response packet from the NTP server
        response, _ = client.recvfrom(1024)
        if response:
            # Unpack the 48-byte NTP header into 12 unsigned 32-bit big-endian integers
            unpacked = struct.unpack("!12I", response)
            
            # The transmit timestamp (index 10) holds the server time in seconds since Jan 1, 1900
            ntp_time: int = unpacked[10] - TIME1900
            
            # Convert epoch seconds into a standard UTC time structure
            struct_time: time.struct_time = time.gmtime(ntp_time)
            date_str: str = time.strftime("%Y-%m-%d %H:%M:%S", struct_time)
            
            print(f"Received time from server: {date_str} UTC")
            
            # Apply time to the operating system software clock via standard date utility
            subprocess.run(["sudo", "date", "-s", date_str], check=True)
            print("System clock successfully updated.")
            
            # Prepare the kernel 'struct rtc_time' binary layout (9 signed integers):
            # Layout order: tm_sec, tm_min, tm_hour, tm_mday, tm_mon, tm_year, tm_wday, tm_yday, tm_isdst
            rtc_data: bytes = struct.pack(
                "9i",
                struct_time.tm_sec,
                struct_time.tm_min,
                struct_time.tm_hour,
                struct_time.tm_mday,
                struct_time.tm_mon - 1,          # Kernel months are 0-indexed (0 = January)
                struct_time.tm_year - 1900,      # Kernel years are relative to 1900
                struct_time.tm_wday,
                struct_time.tm_yday,
                struct_time.tm_isdst
            )
            
            # Open the hardware RTC character device node and execute the ioctl system call
            with open(rtc_device, "wb") as f:
                fcntl.ioctl(f.fileno(), RTC_SET_TIME, rtc_data)
            print(f"Hardware RTC ({rtc_device}) successfully updated.")
            
    except Exception as e:
        print(f"Failed to sync time via NTP: {e}")
        exit(1)
    finally:
        client.close()

if __name__ == "__main__":
    # Accept an optional command-line argument for the target RTC device node,
    # falling back to '/dev/rtc1' if no argument is provided.
    target_rtc: str = sys.argv[1] if len(sys.argv) > 1 else "/dev/rtc1"
    sync_ntp_time(target_rtc)
