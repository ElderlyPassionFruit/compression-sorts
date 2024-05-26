set(target generate_tests_data)

add_executable(${target}    
    main.cpp
)

target_link_libraries(${target} PUBLIC
    compression_sorts_util
    compression_sorts_generator
)

set_target_properties(${target} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BIN_DIR})
