find_package(Threads)
link_libraries(Threads::Threads)

#ZLIB rt dl 
# openssl need
IF(APPLE)
    find_package( ZLIB REQUIRED )
    find_library(security_framework Security) 
    find_library(corefoundation_framework CoreFoundation)
ELSE(APPLE)
    find_package( ZLIB )
    SET(rt_library rt )
    SET(pthread_library pthread)
    set(dl_library dl)
ENDIF(APPLE)
if( ZLIB_FOUND )
    MESSAGE( STATUS "zlib found" )
    add_definitions( -DHAS_ZLIB )
else()
    MESSAGE( STATUS "zlib not found" )
    set( ZLIB_LIBRARIES "" )
endif( ZLIB_FOUND )

#openssl 
message(STATUS "OPENSSL_ROOT_DIR:${OPENSSL_ROOT_DIR}")
if ("${OPENSSL_ROOT_DIR}" STREQUAL "")
    if (NOT "$ENV{OPENSSL_ROOT_DIR}" STREQUAL "")
        set(OPENSSL_ROOT_DIR $ENV{OPENSSL_ROOT_DIR})
        set(OPENSSL_INCLUDE_DIR ${OPENSSL_ROOT_DIR}/include)
    elseif (APPLE)
        set(OPENSSL_ROOT_DIR "/usr/local/opt/openssl")
        set(OPENSSL_INCLUDE_DIR "/usr/local/opt/openssl/include")
    elseif()
        set(OPENSSL_ROOT_DIR "/usr/local/openssl-1.1.1e")
        set(OPENSSL_INCLUDE_DIR "/usr/local/openssl-1.1.1e/include")
    endif()
endif()
message(STATUS "OPENSSL_ROOT_DIR:${OPENSSL_ROOT_DIR}")
SET(OPENSSL_CONF_TARGET )
IF(DEFINED CMAKE_RUNTIME_OUTPUT_DIRECTORY)
    SET (OPENSSL_CONF_TARGET ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
ELSE()
    SET (OPENSSL_CONF_TARGET ${CMAKE_CURRENT_BINARY_DIR})
ENDIF()
set(OPENSSL_USE_STATIC_LIBS TRUE)
find_package(OpenSSL REQUIRED)
message(STATUS "OpenSSL ${OPENSSL_INCLUDE_DIR}")
message(STATUS "OpenSSL ${OPENSSL_LIBRARIES}")

#boost
SET( Boost_USE_STATIC_LIBS ON CACHE STRING "ON or OFF" )
### Remove after Boost 1.70 CMake fixes are in place
# set( Boost_NO_BOOST_CMAKE ON CACHE STRING "ON or OFF" )
find_package(Boost 1.60 REQUIRED COMPONENTS
        log_setup
        log
        date_time
        filesystem
        system
        thread
        regex
        program_options
        chrono
        unit_test_framework
        iostreams
        random
        )
add_definitions(-DBOOST_ASIO_DISABLE_STD_EXPERIMENTAL_STRING_VIEW)
message(STATUS "Boost_LIBRARIES:${Boost_LIBRARIES}")