cmake_minimum_required(VERSION 3.11)
project(timecard LANGUAGES CXX)

include(FetchContent)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

execute_process(
    COMMAND git describe --tags
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_VERSION
    OUTPUT_STRIP_TRAILING_WHITESPACE)
message(STATUS "Timecard version ${GIT_VERSION}")
add_compile_definitions(GIT_VERSION="${GIT_VERSION}")

FetchContent_Declare(
    argon
    GIT_REPOSITORY "https://github.com/ChrisThrasher/argon.git"
    GIT_TAG v0.7.2)
FetchContent_GetProperties(argon)
FetchContent_Populate(argon)
add_subdirectory(${argon_SOURCE_DIR} ${argon_BINARY_DIR} EXCLUDE_FROM_ALL)

add_compile_options(-Werror -Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wsign-conversion)

add_library(core src/CalculateDurations.cpp src/FormatDurations.cpp src/ParseTimepoint.cpp)
target_include_directories(core PUBLIC include)

add_executable(timecard src/Timecard.cpp)
target_link_libraries(timecard PRIVATE core argon::argon)
install(TARGETS timecard DESTINATION bin)

enable_testing()
add_subdirectory(test)
