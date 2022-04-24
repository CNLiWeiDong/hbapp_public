cmake_minimum_required(VERSION 3.5)
project(json_table_plugin)

file(GLOB HEADERS "include/hb/json_table_plugin/*.h"
        )

add_library(json_table_plugin
        json_table_plugin.cpp
        json_table_plugin_impl.cpp
        ${HEADERS}
        )

target_include_directories( json_table_plugin
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" "${CMAKE_CURRENT_BINARY_DIR}/include"
        )

target_link_libraries( json_table_plugin
        appbase
        hb
        log_plugin
        )