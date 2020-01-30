set(driver_source_files
    CO_driver.c
    CO_driver.h
    CO_Linux_tasks.c
    CO_Linux_tasks.h
)

add_library(driver ${driver_source_files})
