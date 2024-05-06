cmake_minimum_required(VERSION 3.28)
project(timecard VERSION 1.9.1 LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

find_package(argon REQUIRED)

if(CMAKE_CXX_COMPILER_ID MATCHES "(GNU|Clang)")
    add_compile_options(-Werror -Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wsign-conversion -Wold-style-cast)
elseif(MSVC)
    add_compile_options(/WX /W4 /permissive- /wd4996)
endif()

add_library(libtimecard)
target_sources(libtimecard PUBLIC FILE_SET CXX_MODULES FILES src/Timecard.cpp)
target_compile_definitions(libtimecard PUBLIC TIMECARD_VERSION="v${timecard_VERSION}")
target_compile_features(libtimecard PUBLIC cxx_std_20)

add_executable(timecard src/Main.cpp)
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
