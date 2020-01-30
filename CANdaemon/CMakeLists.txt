cmake_minimum_required(VERSION 3.13 FATAL_ERROR)
set(CMAKE_C_COMPILER "gcc")
project(CANdaemon)

include_directories(boards/startracker)
include_directories(boards/startracker/objDict)
include_directories(CANopenNode)
include_directories(CANopenNode/stack)
include_directories(common)
include_directories(common/socketCAN)

target_include_directories(mylib PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/mylib>
  $<INSTALL_INTERFACE:include/mylib>  # <prefix>/include/mylib
)

add_subdirectory(boards/startracker)
add_subdirectory(common)

# to not mess with CANopenNode git submodule
set(CANopenNode_source_files
    CANopenNode/CANopen.c
    CANopenNode/CANopen.h
    CANopenNode/stack/CO_Emergency.c
    CANopenNode/stack/CO_Emergency.h
    CANopenNode/stack/CO_HBconsumer.c
    CANopenNode/stack/CO_HBconsumer.h
    CANopenNode/stack/CO_NMT_Heartbeat.c
    CANopenNode/stack/CO_NMT_Heartbeat.h
    CANopenNode/stack/CO_PDO.c
    CANopenNode/stack/CO_PDO.h
    CANopenNode/stack/CO_SDO.c
    CANopenNode/stack/CO_SDO.h
    CANopenNode/stack/CO_SDOmaster.c
    CANopenNode/stack/CO_SDOmaster.h
    CANopenNode/stack/CO_SYNC.c
    CANopenNode/stack/CO_SYNC.h
    CANopenNode/stack/CO_trace.c
    CANopenNode/stack/CO_trace.h
    CANopenNode/stack/crc16-ccitt.c
    CANopenNode/stack/crc16-ccitt.h
)

add_executable(candaemon ${CANopenNode_source_files} ${board_source_files})

target_link_libraries(candaemon common board OD)
