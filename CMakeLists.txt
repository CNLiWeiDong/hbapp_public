cmake_minimum_required(VERSION 3.5)
project(public)

#set constum find_package modele path.
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules")
include(hbapp)
include(publiclib)

add_subdirectory(appbase)
add_subdirectory(hb)
add_subdirectory(plugins)