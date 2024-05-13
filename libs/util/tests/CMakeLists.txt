set(LIBRARIES GTest::gtest_main compression_sorts_util)

set(UNIT_TESTS lz4_test permutation_test suffix_array_test)

foreach(TEST IN LISTS UNIT_TESTS)
    message("test: ${TEST}")
    add_executable(${TEST} "${TEST}.cpp")
    target_link_libraries(${TEST} PUBLIC ${LIBRARIES})
    gtest_discover_tests(${TEST})
endforeach()
