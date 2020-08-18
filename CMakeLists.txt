cmake_minimum_required(VERSION 3.14)
project(timecard)

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

add_executable(timecard src/Timecard.cpp)
target_include_directories(timecard PRIVATE include)
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

FetchContent_Declare(
    googletest
    GIT_REPOSITORY "https://github.com/google/googletest.git"
    GIT_TAG master
)
FetchContent_MakeAvailable(googletest)

enable_testing()
add_executable(unit_tests test/UnitTests.cpp)
target_include_directories(unit_tests PRIVATE include)
target_include_directories(unit_tests SYSTEM PRIVATE ${gtest_SOURCE_DIR}/include)
target_link_libraries(unit_tests PRIVATE gtest)
add_test(NAME unit_tests COMMAND unit_tests)
