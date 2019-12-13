cmake_minimum_required(VERSION 3.5.1)

# specifiy utility source files
set(UTIL_SRCS
        Acquisition.cpp
        Timer.cpp
        )
set(UTIL_HDRS
        Timer.h
        Acquisition.h
        )

add_library(part2_lib_util ${UTIL_HDRS} ${UTIL_SRCS} )
target_link_libraries(part2_lib_util Qt5::Widgets)
