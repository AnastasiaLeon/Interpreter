include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
  dataflowscript_tests
  function_test.cpp
  types_test.cpp
  loop_and_branch_test.cpp
  illegal_ops_test.cpp
  if_else_test.cpp
  system_functions_test.cpp
  number_functions_test.cpp
  string_functions_test.cpp
  list_functions_test.cpp
)

target_link_libraries(
  dataflowscript_tests
  dataflowscript
  GTest::gtest_main
)

target_include_directories(dataflowscript_tests PUBLIC ${PROJECT_SOURCE_DIR})

include(GoogleTest)

gtest_discover_tests(dataflowscript_tests)