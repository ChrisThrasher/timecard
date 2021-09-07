cmake_minimum_required(VERSION 3.16)
project(timecard CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)
FetchContent_Declare(argon
    GIT_REPOSITORY "https://github.com/ChrisThrasher/argon.git"
    GIT_TAG v0.7.4)
FetchContent_MakeAvailable(argon)

execute_process(
    COMMAND git describe --tags
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_VERSION
    OUTPUT_STRIP_TRAILING_WHITESPACE)
message(STATUS "Timecard ${GIT_VERSION}")

if(CMAKE_CXX_COMPILER_ID MATCHES "(GNU|Clang)")
    add_compile_options(-Werror -Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wsign-conversion)
endif()

add_library(libtimecard src/CalculateDurations.cpp src/FormatDurations.cpp src/ParseTimepoint.cpp)
target_include_directories(libtimecard PUBLIC include)
target_compile_definitions(libtimecard PUBLIC GIT_VERSION="${GIT_VERSION}")

add_executable(timecard src/Timecard.cpp)
target_link_libraries(timecard PRIVATE libtimecard argon::argon)
install(TARGETS timecard DESTINATION bin)

enable_testing()
add_subdirectory(tests)

add_custom_target(format
    COMMAND clang-format -i `git ls-files *.cpp *.h`
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
add_custom_target(tidy
    COMMAND clang-tidy -p ${CMAKE_BINARY_DIR} `git ls-files *.cpp`
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
