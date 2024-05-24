set(target compression_sorts_util)

add_library(${target} STATIC
    src/filesystem.cpp
    src/generator.cpp
    src/get_all_files.cpp
    src/lz4.cpp
    src/permutation.cpp
    src/range.cpp
    src/read_data.cpp
    src/serialize_data.cpp
    src/split.cpp
    src/suffix_array.cpp
    src/time.cpp
)

target_include_directories(${target} PUBLIC 
    include
)

find_package(PkgConfig REQUIRED)
pkg_search_module(LZ4 REQUIRED liblz4)

target_include_directories(${target} PUBLIC 
    ${LZ4_INCLUDE_DIRS}
)

target_link_libraries(compression_sorts_util PUBLIC 
    ${LZ4_LIBRARIES}
)

add_subdirectory(tests)
