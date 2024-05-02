set(target benchmark)

add_executable(${target}
    main.cpp
)

target_link_libraries(${target} PUBLIC
    compression_sorts_benchmark
    compression_sorts_data
    compression_sorts_permute_algorithms
    compression_sorts_util
)

set_target_properties(${target} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BIN_DIR})
