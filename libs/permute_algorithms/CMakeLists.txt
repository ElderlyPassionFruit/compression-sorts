set(target compression_sorts_permute_algorithms)

add_library(${target} INTERFACE)

target_include_directories(${target} INTERFACE include)

target_link_libraries(${target} INTERFACE
    compression_sorts_data
)
