file(GLOB SERVER_TEST_SOURCES "${PROJECT_SOURCE_DIR}/test/*/*test.cpp")

##########################################
# "make XYZ_test"
##########################################
foreach (server_test_source ${SERVER_TEST_SOURCES})
    # Create a human readable name.
    get_filename_component(server_test_filename ${server_test_source} NAME)
    string(REPLACE ".cpp" "" server_test_name ${server_test_filename})
    # Add the test target separately and as part of "make check-tests".
    add_executable(${server_test_name} EXCLUDE_FROM_ALL ${server_test_source})
    # add_dependencies(build-tests ${server_test_name})
    # add_dependencies(check-tests ${server_test_name})

    # target_link_libraries(${server_test_name} server_shared gtest gmock_main)
    target_link_libraries(${server_test_name} server_shared)

    # # Set test target properties and dependencies.
    # set_target_properties(${server_test_name}
    #     PROPERTIES
    #     RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/test"
    #     COMMAND ${server_test_name}
    # )

    # # Add the test under CTest.
    # add_test(${server_test_name} ${VALGRIND_BIN} ${VALGRIND_OPTIONS}
    # --suppressions=${VALGRIND_SUPPRESSIONS_FILE} ${CMAKE_BINARY_DIR}/test/${server_test_name}
    # --gtest_color=yes --gtest_output=xml:${CMAKE_BINARY_DIR}/test/unit_${test_name}.xml)
    # add_test(${server_test_name} ${CMAKE_BINARY_DIR}/test/${server_test_name} --gtest_color=yes
    #         --gtest_output=xml:${CMAKE_BINARY_DIR}/test/${server_test_name}.xml)
endforeach(server_test_source ${SERVER_TEST_SOURCES})