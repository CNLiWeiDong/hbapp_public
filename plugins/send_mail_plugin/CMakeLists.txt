cmake_minimum_required(VERSION 3.5)
project(send_mail_plugin)

set(CMAKE_CXX_STANDARD 20)

file(GLOB HEADERS "include/hb/send_mail_plugin/*.h"
        )

add_library(send_mail_plugin
        send_mail_plugin.cpp
        send_mail_plugin_impl.cpp
        session.cpp
        ${HEADERS}
        )

target_include_directories( send_mail_plugin
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" "${CMAKE_CURRENT_BINARY_DIR}/include"
        )

target_link_libraries( send_mail_plugin
        appbase
        hb
        log_plugin
        )