set(board_source_files
    application.c
    application.h
)

add_subdirectory(objDict)

add_library(board ${board_source_files})
