set(target compression_sorts_permute_algorithms)

add_library(
    ${target} STATIC
    src/identity.cpp
    src/lexicographic_sort.cpp
    src/local_optimizations.cpp
    src/multiple_lists.cpp
    src/shuffle.cpp
    src/suffix_array_greedy.cpp
)

target_include_directories(
    ${target} PUBLIC
    include
)

target_link_libraries(
    ${target} PUBLIC
    compression_sorts_data
    compression_sorts_util
)
