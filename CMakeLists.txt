cmake_minimum_required(VERSION 3.16)
project(timecard CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)
FetchContent_Declare(argon
    GIT_REPOSITORY "https://github.com/ChrisThrasher/argon.git"
    GIT_TAG v0.8.0)
FetchContent_MakeAvailable(argon)

execute_process(
    COMMAND git describe --tags
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE TIMECARD_VERSION
    OUTPUT_STRIP_TRAILING_WHITESPACE)
message(STATUS "Timecard ${TIMECARD_VERSION}")

if(CMAKE_CXX_COMPILER_ID MATCHES "(GNU|Clang)")
    add_compile_options(-Werror -Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wsign-conversion)
endif()

add_library(timecard-core src/CalculateDurations.cpp src/FormatDurations.cpp src/ParseTimepoint.cpp)
target_include_directories(timecard-core PUBLIC include)
target_compile_definitions(timecard-core PUBLIC TIMECARD_VERSION="${TIMECARD_VERSION}")
target_compile_features(timecard-core PUBLIC cxx_std_14)

add_executable(timecard src/Timecard.cpp)
target_link_libraries(timecard PRIVATE timecard-core argon::argon)
install(TARGETS timecard)

include(CTest)
add_subdirectory(tests)

add_custom_target(format
    COMMAND clang-format -i `git ls-files *.cpp *.h`
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
add_custom_target(tidy
    COMMAND clang-tidy -p ${CMAKE_BINARY_DIR} `git ls-files *.cpp`
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
