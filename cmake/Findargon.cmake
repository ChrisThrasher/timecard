include(FetchContent)

FetchContent_Declare(argon
    GIT_REPOSITORY https://github.com/ChrisThrasher/argon.git
    GIT_TAG v${argon_FIND_VERSION})
FetchContent_MakeAvailable(argon)
set_target_properties(argon PROPERTIES COMPILE_OPTIONS "" EXPORT_COMPILE_COMMANDS OFF)
