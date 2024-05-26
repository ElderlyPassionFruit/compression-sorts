set(target compression_sorts_generator)

add_library(
    ${target} STATIC
    src/batches.cpp
    src/generators.cpp
)

target_include_directories(
    ${target} PUBLIC
    include
)

target_link_libraries(
    ${target} PUBLIC
    compression_sorts_util
)
