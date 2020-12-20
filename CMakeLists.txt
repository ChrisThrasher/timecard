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
    argon
    GIT_REPOSITORY "https://github.com/ChrisThrasher/argon.git"
    GIT_TAG v0.5.1
)
FetchContent_GetProperties(argon)
FetchContent_Populate(argon)
add_subdirectory(${argon_SOURCE_DIR} ${argon_BINARY_DIR} EXCLUDE_FROM_ALL)

add_executable(timecard src/Timecard.cpp)
target_include_directories(timecard PRIVATE include)
target_link_libraries(timecard PRIVATE argon)
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

add_custom_target(format
    COMMAND ${CMAKE_SOURCE_DIR}/format.sh
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

add_custom_target(analyze
    COMMAND ${CMAKE_SOURCE_DIR}/analyze.sh
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

enable_testing()
add_subdirectory(test)
