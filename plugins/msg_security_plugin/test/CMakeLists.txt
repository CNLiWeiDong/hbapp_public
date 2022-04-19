add_executable( msg_security_test msg_security_test.cpp )

target_link_libraries( msg_security_test appbase hb log_plugin msg_security_plugin ${Boost_LIBRARIES} pthread)

add_test(NAME msg_security_test COMMAND msg_security_test WORKING_DIRECTORY ${CMAKE_BINARY_DIR})