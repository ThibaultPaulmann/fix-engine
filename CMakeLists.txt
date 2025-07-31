# CMakeLists.txt
cmake_minimum_required(VERSION 3.16)
project(FixParser)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
include_directories(include)

# Core sources
file(GLOB_RECURSE CORE_SRC src/core/*.cpp)
file(GLOB_RECURSE ENGINE_SRC src/engine/*.cpp)
file(GLOB_RECURSE PROTOCOL_SRC src/protocol/*.cpp)

add_library(fix STATIC ${CORE_SRC} ${ENGINE_SRC} ${PROTOCOL_SRC})

# Tests
enable_testing()
add_subdirectory(tests)