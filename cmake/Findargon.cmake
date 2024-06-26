include(FetchContent)

FetchContent_Declare(argon
    GIT_REPOSITORY https://github.com/ChrisThrasher/argon.git
    GIT_TAG v${argon_FIND_VERSION}
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(argon)
set_target_properties(argon PROPERTIES EXPORT_COMPILE_COMMANDS OFF)
