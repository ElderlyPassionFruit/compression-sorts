set(target compression_sorts_benchmark)

add_library(
    ${target} STATIC
    src/benchmark.cpp
    src/statistics_saver.cpp
    src/test_results.cpp
    src/tests.cpp
)

target_include_directories(
    ${target} PUBLIC
    include
)

target_link_libraries(
    ${target} PUBLIC
    compression_sorts_util
    compression_sorts_data
    compression_sorts_permute_algorithms
)
