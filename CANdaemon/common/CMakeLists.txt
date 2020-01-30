set(common_source_files
    CO_time.c
    CO_time.h
    error_assert_handlers.c
    error_assert_handlers.h
    file_transfer_ODF.c
    file_transfer_ODF.h
    main.c
    OD_helpers.c
    OD_helpers.h
    systemd_ODF.c
    systemd_ODF.h
    updater.c
    updater.h
)

add_subdirectory(socketCAN)

add_library(common ${common_source_files})
