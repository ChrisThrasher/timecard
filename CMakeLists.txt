cmake_minimum_required(VERSION 3.16)
project(timecard VERSION 1.9.0 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

find_package(argon 0.9.1 REQUIRED)

if(CMAKE_CXX_COMPILER_ID MATCHES "(GNU|Clang)")
    add_compile_options(-Werror -Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wsign-conversion -Wold-style-cast)
endif()

add_library(timecard-core STATIC src/CalculateDurations.cpp src/FormatDurations.cpp src/ParseTimepoint.cpp)
target_include_directories(timecard-core PUBLIC include)
target_compile_definitions(timecard-core PUBLIC TIMECARD_VERSION="v${timecard_VERSION}")
target_compile_features(timecard-core PUBLIC cxx_std_17)

add_executable(timecard src/Timecard.cpp)
target_link_libraries(timecard PRIVATE timecard-core argon::argon)
install(TARGETS timecard)

include(CTest)
add_subdirectory(tests)

add_custom_target(format
    COMMAND clang-format -i `git ls-files *.hpp *.cpp`
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
add_custom_target(tidy
    COMMAND clang-tidy -p ${CMAKE_BINARY_DIR} `git ls-files *.cpp`
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
