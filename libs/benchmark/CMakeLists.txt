set(target compression_sorts_benchmark)

add_library(${target} STATIC
    src/statistics_saver.cpp
)

target_include_directories(${target} PUBLIC include
)

target_link_libraries(${target} PRIVATE
    compression_sorts_util
    compression_sorts_permute_algorithms
)
