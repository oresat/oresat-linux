# rtc_diag.py - READ-ONLY. Ask the RTC driver for its voltage-low flags and
# its backup switchover mode. Changes nothing.
# usage: python3 rtc_diag.py <rtc-device>
import fcntl, struct, sys

# ---- command-line layout ----
ARG_DEVICE = 1
EXPECTED_ARG_COUNT = 2       # program name + one argument
USAGE = "usage: rtc_diag.py <rtc-device>"

# ---- exit codes ----
EXIT_OK = 0
EXIT_USAGE = 1
EXIT_DEVICE_ERROR = 2

# ---- how a Linux ioctl request number is built ----
IOC_WRITE = 1                # data goes from program to kernel
IOC_READ = 2                 # data comes from kernel to program
IOC_DIR_SHIFT = 30
IOC_SIZE_SHIFT = 16
IOC_TYPE_SHIFT = 8
RTC_IOC_TYPE = ord('p')      # the RTC family letter

# ---- request numbers within the RTC family (from include/uapi/linux/rtc.h) ----
RTC_VL_READ_NUMBER = 0x13    # read voltage-low flags
RTC_PARAM_GET_NUMBER = 0x13  # read a driver parameter

# ---- data layouts ----
VL_FORMAT = 'I'              # one unsigned int of flag bits
RTC_PARAM_FORMAT = 'QQII'    # param (u64), value (u64), index (u32), pad (u32)
RTC_PARAM_BACKUP_SWITCH_MODE = 2

# ---- voltage-low flag bits ----
VL_BIT_NAMES = {
    0: "data invalid (chip lost power; power-on flag is set)",
    1: "backup voltage low",
    2: "backup empty or not present",
    3: "accuracy low",
    4: "backup switchover happened",
}

# ---- backup switchover modes ----
BSM_NAMES = {0: "disabled", 1: "direct", 2: "level", 3: "standby"}


def ioc(direction: int, number: int, size: int) -> int:
    """Build an ioctl request number from its four fields."""
    return ((direction << IOC_DIR_SHIFT)
            | (size << IOC_SIZE_SHIFT)
            | (RTC_IOC_TYPE << IOC_TYPE_SHIFT)
            | number)


RTC_VL_READ = ioc(IOC_READ, RTC_VL_READ_NUMBER, struct.calcsize(VL_FORMAT))
RTC_PARAM_GET = ioc(IOC_WRITE, RTC_PARAM_GET_NUMBER,
                    struct.calcsize(RTC_PARAM_FORMAT))


def read_voltage_low_flags(fd: int) -> int:
    buf = bytearray(struct.calcsize(VL_FORMAT))
    fcntl.ioctl(fd, RTC_VL_READ, buf, True)       # True = let the kernel fill buf
    return struct.unpack(VL_FORMAT, buf)[0]


def read_backup_switch_mode(fd: int) -> int:
    buf = bytearray(struct.pack(RTC_PARAM_FORMAT,
                                RTC_PARAM_BACKUP_SWITCH_MODE, 0, 0, 0))
    fcntl.ioctl(fd, RTC_PARAM_GET, buf, True)
    return struct.unpack(RTC_PARAM_FORMAT, buf)[1]  # field 1 = the value


def main(argv: list) -> int:
    if len(argv) != EXPECTED_ARG_COUNT:
        print(USAGE, file=sys.stderr)
        return EXIT_USAGE
    device = argv[ARG_DEVICE]
    try:
        f = open(device, 'r')
    except OSError as err:
        print("could not open %s: %s" % (device, err), file=sys.stderr)
        return EXIT_DEVICE_ERROR
    status = EXIT_OK
    with f:
        try:
            flags = read_voltage_low_flags(f.fileno())
            print("voltage-low flags: 0x%x" % flags)
            for bit, name in VL_BIT_NAMES.items():
                if flags & (1 << bit):
                    print("  SET:", name)
            if flags == 0:
                print("  none set")
        except OSError as err:
            print("voltage-low read failed:", err)
            status = EXIT_DEVICE_ERROR
        try:
            mode = read_backup_switch_mode(f.fileno())
            print("backup switchover mode: %d (%s)"
                  % (mode, BSM_NAMES.get(mode, "unknown")))
        except OSError as err:
            print("backup switchover read failed:", err)
            status = EXIT_DEVICE_ERROR
    return status


if __name__ == "__main__":
    sys.exit(main(sys.argv))
