cmake_minimum_required(VERSION 3.22)
project(timecard VERSION 1.9.1 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

find_package(argon 0.10.0 REQUIRED)

if(CMAKE_CXX_COMPILER_ID MATCHES "(GNU|Clang)")
    add_compile_options(-Werror -Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wsign-conversion -Wold-style-cast)
elseif(MSVC)
    add_compile_options(/WX /W4 /permissive- /wd4996)
endif()

add_library(libtimecard STATIC src/CalculateDurations.cpp src/FormatDurations.cpp src/ParseTimepoint.cpp)
target_include_directories(libtimecard PUBLIC include)
target_compile_definitions(libtimecard PUBLIC TIMECARD_VERSION="v${timecard_VERSION}")
target_compile_features(libtimecard PUBLIC cxx_std_17)
set_target_properties(libtimecard PROPERTIES OUTPUT_NAME timecard)

add_executable(timecard src/Timecard.cpp)
target_link_libraries(timecard PRIVATE libtimecard argon::argon)
install(TARGETS timecard)

include(CTest)
if(BUILD_TESTING)
    add_subdirectory(tests)
endif()

add_custom_target(format
    COMMAND clang-format -i `git ls-files *.hpp *.cpp`
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
add_custom_target(tidy COMMAND run-clang-tidy -p ${CMAKE_BINARY_DIR})
