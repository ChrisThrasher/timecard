if(NOT BUILD_TESTING)
    return()
endif()

FetchContent_Declare(Catch2
    GIT_REPOSITORY "https://github.com/catchorg/Catch2.git"
    GIT_TAG v2.13.7)
FetchContent_MakeAvailable(Catch2)

add_executable(unit_tests UnitTests.cpp)
target_link_libraries(unit_tests PRIVATE timecard-core Catch2::Catch2)
add_test(NAME unit_tests COMMAND unit_tests)

add_test(NAME flag--help COMMAND timecard --help)
set_property(TEST flag--help PROPERTY PASS_REGULAR_EXPRESSION "Usage.*")

add_test(NAME flag--version COMMAND timecard --version)
set_property(TEST flag--version PROPERTY PASS_REGULAR_EXPRESSION "${GIT_VERSION}")
