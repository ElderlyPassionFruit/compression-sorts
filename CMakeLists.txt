cmake_minimum_required(VERSION 3.13)
project(compression-sorts)

set(CMAKE_CXX_STANDARD 20)
set(CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

message("C++ version: ${CMAKE_CXX_STANDARD}")
message("C++ compiler: ${CMAKE_CXX_COMPILER_ID}")
message("build type: ${CMAKE_BUILD_TYPE}")

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -fsanitize=address,undefined,leak -Wall -Wextra -Wno-unsigned-compare -Wno-unused-result -g")
    set(CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fsanitize=address,undefined,leak")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -Wextra -Wno-sign-compare -Wno-unused-result -Werror")
endif()

message("compile flags: ${CMAKE_CXX_FLAGS}")

find_package(PkgConfig REQUIRED)
pkg_search_module(LZ4 REQUIRED liblz4)

# include(GoogleTest)
# enable_testing()

set(PROJECT_BIN_DIR ${CMAKE_CURRENT_SOURCE_DIR}/bin)

add_subdirectory(libs)

add_subdirectory(benchmark)
