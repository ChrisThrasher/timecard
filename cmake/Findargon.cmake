include(FetchContent)

FetchContent_Declare(argon
    GIT_REPOSITORY https://github.com/ChrisThrasher/argon.git
    GIT_TAG v${argon_FIND_VERSION})
FetchContent_MakeAvailable(argon)
set_target_properties(argon PROPERTIES COMPILE_OPTIONS "")
get_target_property(ARGON_INCLUDE_DIRS argon INTERFACE_INCLUDE_DIRECTORIES)
target_include_directories(argon SYSTEM INTERFACE ${ARGON_INCLUDE_DIRS})
