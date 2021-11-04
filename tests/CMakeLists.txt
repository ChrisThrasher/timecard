if(NOT BUILD_TESTING)
    return()
endif()

FetchContent_Declare(Catch2
    GIT_REPOSITORY "https://github.com/catchorg/Catch2.git"
    GIT_TAG v2.13.7)
FetchContent_MakeAvailable(Catch2)

add_executable(test-timecard-core UnitTests.cpp)
target_link_libraries(test-timecard-core PRIVATE timecard-core Catch2::Catch2)
add_test(NAME test-timecard-core COMMAND test-timecard-core)

add_test(NAME test-help-flag COMMAND timecard --help)
set_property(TEST test-help-flag PROPERTY PASS_REGULAR_EXPRESSION "Usage.*")

add_test(NAME test-version-flag COMMAND timecard --version)
set_property(TEST test-version-flag PROPERTY PASS_REGULAR_EXPRESSION "${GIT_VERSION}")
