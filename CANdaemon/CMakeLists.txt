cmake_minimum_required(VERSION 3.13 FATAL_ERROR)
project(CANdaemon C)

##############################################################################
# Release vs debug modes (set CFLAGS, LDFLAGS, etc)
# to use: cmake -DCMAKE_BUILD_TYPE=Debug . 

# default to release
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_C_FLAGS "-Wall -Wextra")
set(CMAKE_C_FLAGS_DEBUG "-g")
set(CMAKE_C_FLAGS_RELEASE "-O3")

set(CMAKE_EXE_LINKER_FLAGS "-pthread -lsystemd")

##############################################################################
# Check Linux distribution / verion / kernel version

set(LINUX_DISTRIBUTION "")

if(EXISTS "/etc/os-release")
    execute_process(COMMAND cat /etc/os-release
        COMMAND grep -m1 NAME
        COMMAND awk -F= "{ print $2 }"
        COMMAND tr "\n" " "
        COMMAND sed "s/ //"
        OUTPUT_VARIABLE LINUX_DISTRIBUTION
        RESULT_VARIABLE LINUX_ID_RESULT
    )
    if(${LINUX_DISTRIBUTION} MATCHES "Debian")
        set(LINUX_DISTRIBUTION "Debian")
    endif()
else()
    message(FATAL_ERROR "Missing /etc/os-release. Not a Linux based system?")
endif()

if(LINUX_DISTRIBUTION MATCHES "Debian")
    find_library(DEB_LIBSYSTEMD libsystemd-dev)
    if(NOT DEB_LIBSYSTEMD)
        message(FATAL_ERROR "Debian libsystemd library not found")
    endif()
endif()

##############################################################################
# Check if make or ninja

if(CMAKE_MAKE_PROGRAM MATCHES "make") 
    set(CMAKE_MAKE_PROGRAM_PRETTY_NAME "make")
endif()
if(CMAKE_MAKE_PROGRAM MATCHES "ninja")
    set(CMAKE_MAKE_PROGRAM_PRETTY_NAME "ninja")
endif()
if(NOT CMAKE_MAKE_PROGRAM_PRETTY_NAME)
    set(CMAKE_MAKE_PROGRAM_PRETTY_NAME "${CMAKE_MAKE_PROGRAM}")
endif()

##############################################################################
# Figure out which board

# TODO change later
set(BOARD startracker)
if(NOT BOARD)
    message(FATAL_ERROR "specfy board\n ie: cmake -DBOARD=startracker .")
endif()

##############################################################################
# Get source files

# all paths to source directories
set(BOARDS_DIR boards/${BOARD})
set(OD_DIR boards/${BOARD}/objDict)
set(CANOPENNODE_DIR CANopenNode)
set(STACK_DIR CANopenNode/stack)
set(COMMON_DIR common)
set(STACK_DRIVER_DIR common/socketCAN)

# add all headers in source directories
include_directories(${BOARDS_DIR})
include_directories(${OD_DIR})
include_directories(${CANOPENNODE_DIR})
include_directories(${STACK_DIR})
include_directories(${STACK_DRIVER_DIR})
include_directories(${COMMON_DIR})

set(SOURCE_FILES
    ${BOARDS_DIR}/application.c
    ${OD_DIR}/CO_OD.c
    ${CANOPENNODE_DIR}/CANopen.c
    ${STACK_DIR}/CO_Emergency.c
    ${STACK_DIR}/CO_HBconsumer.c
    ${STACK_DIR}/CO_NMT_Heartbeat.c
    ${STACK_DIR}/CO_PDO.c
    ${STACK_DIR}/CO_SDO.c
    ${STACK_DIR}/CO_SDOmaster.c
    ${STACK_DIR}/CO_SYNC.c
    ${STACK_DIR}/CO_trace.c
    ${STACK_DIR}/crc16-ccitt.c
    ${STACK_DRIVER_DIR}/CO_driver.c
    ${STACK_DRIVER_DIR}/CO_Linux_tasks.c
    ${COMMON_DIR}/CO_time.c
    ${COMMON_DIR}/error_assert_handlers.c
    ${COMMON_DIR}/file_transfer_ODF.c
    ${COMMON_DIR}/main.c
    ${COMMON_DIR}/OD_helpers.c
    ${COMMON_DIR}/systemd_ODF.c
    ${COMMON_DIR}/updater.c
)

##############################################################################
# Create an executable

add_executable(candaemon ${SOURCE_FILES})

############################################################
# Install

# Binaries
install (TARGETS candaemon DESTINATION bin)

# Daemon service file
install (FILES oresat-candaemon.service DESTINATION etc/systemd/system)


##############################################################################
# Cpack Debian Package

if(MAKE_PKG) # TODO remove if
    set(CPACK_PACKAGE_NAME candaemon)
    set(CPACK_PACKAGE_VERSION 0.0.1) # TODO link to github version??
    set(CPACK_DEBIAN_PACKAGE_DEPENDS "cmake (), make (), gcc (), libsystemd ()") # TODO finish
    set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Ryan Medick")
    set(CPACK_DEBIAN_PACKAGE_DESCRIPTION "CAN systemd controller")
    set(CPACK_DEBIAN_PACKAGE_SECTION "embedded")
    set(CPACK_DEBIAN_PACKAGE_HOMEPAGE "http://www.oresat.org")
    set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE all)
    set(CPACK_DEBIAN_PACKAGE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${CPACK_DEBIAN_PACKAGE_ARCHITECTURE}")
endif()

