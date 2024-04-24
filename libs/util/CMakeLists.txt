set(target compression_sorts_util)

add_library(${target} STATIC
    src/get_all_files.cpp
    src/lz4.cpp
    src/permutation.cpp
    src/time.cpp
)

target_include_directories(${target} PUBLIC 
    include
)

find_package(PkgConfig REQUIRED)
pkg_search_module(LZ4 REQUIRED liblz4)

target_include_directories(${target} PRIVATE 
    ${LZ4_INCLUDE_DIRS}
)

target_link_libraries(${target} ${LZ4_LIBRARIES})
