option(BUILD_GMOCK "Build Google Mock" off)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.11.0
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(unit_tests tree_test.cpp ${PROJECT_SOURCE_DIR}/src/Staff/Staff.cpp)
target_link_libraries(unit_tests PRIVATE lib gtest_main)
target_include_directories(unit_tests PRIVATE ${PROJECT_SOURCE_DIR}/src/Staff)
include(GoogleTest)
gtest_discover_tests(unit_tests)

