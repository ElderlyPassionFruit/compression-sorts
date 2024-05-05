set(target compression_sorts_permute_algorithms)

add_library(${target} STATIC
    src/identity.cpp
    src/lexicographic_sort.cpp
    src/local_optimizations.cpp
    src/shuffle.cpp
)

target_include_directories(${target} PUBLIC include)

target_link_libraries(${target} PUBLIC
    compression_sorts_data
    compression_sorts_util
)
