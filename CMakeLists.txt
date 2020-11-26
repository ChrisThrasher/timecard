cmake_minimum_required(VERSION 3.11)
project(timecard LANGUAGES CXX)

include(FetchContent)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_BUILD_TYPE Release)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

execute_process(
    COMMAND git describe --tags
    OUTPUT_VARIABLE GIT_VERSION
    OUTPUT_STRIP_TRAILING_WHITESPACE)
message(STATUS "Timecard version ${GIT_VERSION}")
add_compile_definitions(GIT_VERSION="${GIT_VERSION}")

FetchContent_Declare(
    options
    GIT_REPOSITORY "https://github.com/ChrisThrasher/options.git"
    GIT_TAG v0.5.0
)
FetchContent_GetProperties(options)
FetchContent_Populate(options)
add_subdirectory(${options_SOURCE_DIR} ${options_BINARY_DIR} EXCLUDE_FROM_ALL)

add_executable(timecard src/Timecard.cpp)
target_include_directories(timecard PRIVATE include)
target_link_libraries(timecard PRIVATE options)
target_compile_options(timecard PRIVATE
    -Werror
    -Wall
    -Wextra
    -Wpedantic
    -Wshadow
    -Wconversion
    -Wsign-conversion
    -Wold-style-cast
)
install(TARGETS timecard DESTINATION bin)

enable_testing()
add_subdirectory(test)