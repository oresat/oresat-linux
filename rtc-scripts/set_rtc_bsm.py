# set_rtc_bsm.py - put an RTC's backup switchover mode into LEVEL (LSM).
#
# Default is a DRY RUN: it only reads and reports. Nothing is written unless
# you add --write. A write goes into the chip's EEPROM (limited write cycles),
# so the script reads first and skips the write if the mode is already LEVEL.
#
# usage: python3 set_rtc_bsm.py <rtc-device> [--write]
import fcntl, struct, sys

# ---- command-line layout ----
ARG_PROGRAM_NAME = 0
ARG_DEVICE = 1
ARG_WRITE_FLAG = 2
MIN_ARG_COUNT = 2            # program name + device
MAX_ARG_COUNT = 3            # ... + optional --write
WRITE_FLAG = "--write"
USAGE = "usage: set_rtc_bsm.py <rtc-device> [--write]"

# ---- exit codes ----
EXIT_OK = 0
EXIT_USAGE = 1
EXIT_DEVICE_ERROR = 2
EXIT_VERIFY_FAILED = 3

# ---- how a Linux ioctl request number is built (see ioctl.h) ----
IOC_WRITE = 1                # data goes from program to kernel
IOC_READ = 2                 # data comes from kernel to program
IOC_DIR_SHIFT = 30
IOC_SIZE_SHIFT = 16
IOC_TYPE_SHIFT = 8
RTC_IOC_TYPE = ord('p')      # the RTC family letter

# ---- request numbers within the RTC family (include/uapi/linux/rtc.h) ----
RTC_VL_READ_NUMBER = 0x13    # read voltage-low flags
RTC_PARAM_GET_NUMBER = 0x13  # read a driver parameter
RTC_PARAM_SET_NUMBER = 0x14  # write a driver parameter

# ---- data layouts ----
VL_FORMAT = 'I'              # one unsigned int of flag bits
RTC_PARAM_FORMAT = 'QQII'    # param (u64), value (u64), index (u32), pad (u32)
PARAM_INDEX_UNUSED = 0
PARAM_PAD_UNUSED = 0
RTC_PARAM_BACKUP_SWITCH_MODE = 2

# ---- Linux's numbering for backup switchover modes (NOT the chip's bits) ----
RTC_BSM_DISABLED = 0
RTC_BSM_DIRECT = 1
RTC_BSM_LEVEL = 2
RTC_BSM_STANDBY = 3
BSM_NAMES = {RTC_BSM_DISABLED: "disabled", RTC_BSM_DIRECT: "direct (DSM)",
             RTC_BSM_LEVEL: "level (LSM)", RTC_BSM_STANDBY: "standby"}
TARGET_MODE = RTC_BSM_LEVEL

# ---- voltage-low flag bit 0 = the power-on flag (PORF) ----
VL_DATA_INVALID_BIT = 0


def ioc(direction: int, number: int, size: int) -> int:
    """Build an ioctl request number from its four fields."""
    return ((direction << IOC_DIR_SHIFT)
            | (size << IOC_SIZE_SHIFT)
            | (RTC_IOC_TYPE << IOC_TYPE_SHIFT)
            | number)


RTC_PARAM_SIZE = struct.calcsize(RTC_PARAM_FORMAT)
RTC_VL_READ = ioc(IOC_READ, RTC_VL_READ_NUMBER, struct.calcsize(VL_FORMAT))
RTC_PARAM_GET = ioc(IOC_WRITE, RTC_PARAM_GET_NUMBER, RTC_PARAM_SIZE)
RTC_PARAM_SET = ioc(IOC_WRITE, RTC_PARAM_SET_NUMBER, RTC_PARAM_SIZE)


def read_voltage_low_flags(fd: int) -> int:
    buf = bytearray(struct.calcsize(VL_FORMAT))
    fcntl.ioctl(fd, RTC_VL_READ, buf, True)       # True = let the kernel fill buf
    return struct.unpack(VL_FORMAT, buf)[0]


def read_backup_switch_mode(fd: int) -> int:
    buf = bytearray(struct.pack(RTC_PARAM_FORMAT, RTC_PARAM_BACKUP_SWITCH_MODE,
                                0, PARAM_INDEX_UNUSED, PARAM_PAD_UNUSED))
    fcntl.ioctl(fd, RTC_PARAM_GET, buf, True)
    return struct.unpack(RTC_PARAM_FORMAT, buf)[1]  # field 1 = the value


def write_backup_switch_mode(fd: int, mode: int) -> None:
    """Ask the driver to store a new switchover mode (writes the chip EEPROM)."""
    request = struct.pack(RTC_PARAM_FORMAT, RTC_PARAM_BACKUP_SWITCH_MODE,
                          mode, PARAM_INDEX_UNUSED, PARAM_PAD_UNUSED)
    fcntl.ioctl(fd, RTC_PARAM_SET, request)


def mode_name(mode: int) -> str:
    return BSM_NAMES.get(mode, "unknown")


def report_power_on_flag(fd: int) -> None:
    flags = read_voltage_low_flags(fd)
    if flags & (1 << VL_DATA_INVALID_BIT):
        print("power-on flag (PORF): SET - time is invalid, set the time again")
    else:
        print("power-on flag (PORF): clear - time is valid")


def main(argv: list) -> int:
    if not MIN_ARG_COUNT <= len(argv) <= MAX_ARG_COUNT:
        print(USAGE, file=sys.stderr)
        return EXIT_USAGE
    write_requested = False
    if len(argv) == MAX_ARG_COUNT:
        if argv[ARG_WRITE_FLAG] != WRITE_FLAG:
            print(USAGE, file=sys.stderr)
            return EXIT_USAGE
        write_requested = True
    device = argv[ARG_DEVICE]

    try:
        f = open(device, 'r')
    except OSError as err:
        print("could not open %s: %s (writing needs sudo)" % (device, err),
              file=sys.stderr)
        return EXIT_DEVICE_ERROR

    with f:
        fd = f.fileno()
        try:
            report_power_on_flag(fd)
            current = read_backup_switch_mode(fd)
            print("backup switchover mode now: %d (%s)"
                  % (current, mode_name(current)))

            if current == TARGET_MODE:
                print("already %s - nothing to write" % mode_name(TARGET_MODE))
                return EXIT_OK
            if not write_requested:
                print("DRY RUN: would set %s. Nothing written. Add %s to apply."
                      % (mode_name(TARGET_MODE), WRITE_FLAG))
                return EXIT_OK

            print("writing %s to the chip EEPROM ..." % mode_name(TARGET_MODE))
            write_backup_switch_mode(fd, TARGET_MODE)
            after = read_backup_switch_mode(fd)
            print("backup switchover mode after write: %d (%s)"
                  % (after, mode_name(after)))
            report_power_on_flag(fd)
        except OSError as err:
            print("device error: %s" % err, file=sys.stderr)
            return EXIT_DEVICE_ERROR

    if after != TARGET_MODE:
        print("VERIFY FAILED: expected %s" % mode_name(TARGET_MODE),
              file=sys.stderr)
        return EXIT_VERIFY_FAILED
    return EXIT_OK


if __name__ == "__main__":
    sys.exit(main(sys.argv))
