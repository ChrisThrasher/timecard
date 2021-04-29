cmake_minimum_required(VERSION 3.14)
project(timecard CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)
FetchContent_Declare(argon
    GIT_REPOSITORY "https://github.com/ChrisThrasher/argon.git"
    GIT_TAG v0.7.2)
FetchContent_MakeAvailable(argon)

execute_process(
    COMMAND git describe --tags
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_VERSION
    OUTPUT_STRIP_TRAILING_WHITESPACE)
message(STATUS "Timecard version ${GIT_VERSION}")
add_compile_definitions(GIT_VERSION="${GIT_VERSION}")

add_compile_options(-Werror -Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wsign-conversion)

add_library(timecard-core src/CalculateDurations.cpp src/FormatDurations.cpp src/ParseTimepoint.cpp)
target_include_directories(timecard-core PUBLIC include)

add_executable(timecard src/Timecard.cpp)
target_link_libraries(timecard PRIVATE timecard-core argon::argon)
install(TARGETS timecard DESTINATION bin)

add_custom_target(format
    COMMAND clang-format -i `git ls-files src/* include/*`
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
add_custom_target(tidy
    COMMAND clang-tidy -p build/compile_commands.json `git ls-files src/*.cpp`
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

enable_testing()
add_subdirectory(test)
