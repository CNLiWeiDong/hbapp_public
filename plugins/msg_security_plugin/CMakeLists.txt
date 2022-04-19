cmake_minimum_required(VERSION 3.5)
project(msg_security_plugin)

set(CMAKE_CXX_STANDARD 20)

file(GLOB HEADERS "include/hb/msg_security_plugin/*.h"
        )

add_library(msg_security_plugin
        msg_security_plugin.cpp
        msg_security_plugin_impl.cpp
        ${HEADERS}
        )

target_include_directories(msg_security_plugin
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" "${CMAKE_CURRENT_BINARY_DIR}/include"
        )

target_link_libraries(msg_security_plugin
        log_plugin
        appbase
        hb
        )

IF(NOT DEFINED SKIP_HB_TESTS)
        add_subdirectory( test )
ENDIF()