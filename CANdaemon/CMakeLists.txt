cmake_minimum_required(VERSION 3.13 FATAL_ERROR)
project(CANdaemon C)

set(CMAKE_EXE_LINKER_FLAGS "-pthread -lsystemd")

# TODO change later
set(BOARD startracker)

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

#target_link_libraries(pthread_Mutex pthread)
#target_compile_options(candaemon PUBLIC "-pthread")

add_executable(candaemon ${SOURCE_FILES})
