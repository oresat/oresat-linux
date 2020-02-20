cmake_minimum_required(VERSION 3.13 FATAL_ERROR)
project(CANdaemon C)


##############################################################################
# Figure out which board

if(NOT BOARD)
    message(FATAL_ERROR "No board specified\n  ie: cmake -DBOARD=star-tracker .")
endif()

# ignore case
STRING(TOLOWER "${BOARD}" BOARD)

if(BOARD MATCHES "cirrus-flux-camera" OR BOARD MATCHES "cirrus_flux_camera" OR BOARD MATCHES "cfc")
    set(BOARD cirrus_flux_camera)
elseif(BOARD MATCHES "gps")
elseif(BOARD MATCHES "live")
elseif(BOARD MATCHES "star-tracker" OR BOARD MATCHES "star_tracker" OR BOARD MATCHES "st")
    set(BOARD star_tracker)
elseif(BOARD MATCHES "template")
else()
    message(FATAL_ERROR "${BOARD} is not a valid board")
endif()

message(STATUS "Board ${BOARD} selected")


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


##############################################################################
# Deal with dependencies

# pthreads
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# pkg-config
find_package(PkgConfig REQUIRED)

# libsystemd
pkg_check_modules(LIBSYSTEMD REQUIRED libsystemd)


##############################################################################
# Deal with git submodule

find_package(Git)
if(NOT GIT_FOUND)
    message(FATAL_ERROR "git check failed")
endif()
if(NOT EXISTS "./src/CANopenNode/.git")
    message(STATUS "Running: git submoduke update --init")
    execute_process(COMMAND git submodule update --init WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
endif()


##############################################################################
# Release vs debug modes (set CFLAGS, LDFLAGS, etc)
# to use: cmake -DCMAKE_BUILD_TYPE=Debug .

# default to release
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_C_FLAGS "-Wall")
set(CMAKE_C_FLAGS_DEBUG "-g")
set(CMAKE_C_FLAGS_RELEASE "-O3")

#set(CMAKE_EXE_LINKER_FLAGS "-pthread") #-lsystemd")


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
# Get source files

# all paths to source directories
set(BOARDS_DIR src/boards/${BOARD})
set(OD_DIR src/boards/${BOARD}/object_dictionary)
set(CANOPENNODE_DIR src/CANopenNode)
set(STACK_DIR src/CANopenNode/stack)
set(COMMON_DIR src/common)
set(STACK_DRIVER_DIR src/socketCAN)

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
    ${COMMON_DIR}/log_message.c
    ${COMMON_DIR}/file_transfer_ODF.c
    ${COMMON_DIR}/OD_helpers.c
    ${COMMON_DIR}/systemd_ODF.c
    ${COMMON_DIR}/updater.c
    src/main.c
)


##############################################################################
# Create an executable

add_executable(candaemon ${SOURCE_FILES})
target_link_libraries(candaemon ${LIBSYSTEMD_LIBRARIES} Threads::Threads)
target_include_directories(candaemon PUBLIC ${LIBSYSTEMD_INCLUDE_DIRS})
target_compile_options(candaemon PUBLIC ${LIBSYSTEMD_CFLAGS_OTHER})


############################################################
# Install

# Binaries
install(TARGETS candaemon DESTINATION /usr/bin)

# Daemon service file
install(FILES oresat-candaemon.service DESTINATION /lib/systemd/system)


##############################################################################
# Cpack debian package

if(${LINUX_DISTRIBUTION} MATCHES "Debian")
    set(CPACK_GENERATOR "DEB")
    set(CPACK_PACKAGE_VENDOR "Company")
    set(CPACK_COMPONENTS_ALL exe link config lastversion script log monitor)
    set(CPACK_PACKAGE_NAME "oresat-candaemon-${BOARD}")
    set(CPACK_PACKAGE_VERSION 0.0.1) # TODO link to github version??
    set(CPACK_PACKAGE_CONTACT "rmedick@pdx.edu")
    set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "CAN systemd controller")
    set(CPACK_DEBIAN_PACKAGE_DEPENDS "libsystemd-dev (>= 241-7~deb10u2rcnee0~buster+20191124)")
    set(CPACK_DEBIAN_PACKAGE_SECTION "embedded")
    set(CPACK_DEBIAN_PACKAGE_HOMEPAGE "http://www.oresat.org")
    execute_process(COMMAND dpkg --print-architecture OUTPUT_VARIABLE CPACK_DEBIAN_PACKAGE_ARCHITECTURE OUTPUT_STRIP_TRAILING_WHITESPACE)
    set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${CPACK_DEBIAN_PACKAGE_ARCHITECTURE}")

    include(CPack)
endif()

