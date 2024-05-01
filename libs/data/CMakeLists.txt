set(target compression_sorts_data)

add_library(${target} STATIC
    src/block.cpp
)

target_include_directories(${target} PUBLIC 
    include
)

target_link_libraries(${target} PRIVATE
    compression_sorts_util
)
