set(target compression_sorts_data)

add_library(
    ${target} STATIC
    src/block.cpp
    src/column_strings.cpp
)

target_include_directories(
    ${target} PUBLIC
    include
)

target_link_libraries(
    ${target} PUBLIC
    compression_sorts_util
)

add_subdirectory(tests)
