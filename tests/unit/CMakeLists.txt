message(STATUS "---------------------------------------------------------------")
message(STATUS "[ 🛠️ ] Building Unit Tests")
message(STATUS "---------------------------------------------------------------")

find_package(Catch2 CONFIG REQUIRED)

add_executable(unit_color_tests
  src/color.tests.cpp
)

add_executable(unit_size_tests
  src/size.tests.cpp
)

target_link_libraries(unit_color_tests PRIVATE
  Catch2::Catch2WithMain
  ${PROJECT_NAME}
)

target_link_libraries(unit_size_tests PRIVATE
  Catch2::Catch2WithMain
  ${PROJECT_NAME}
)

set_target_properties(unit_color_tests PROPERTIES
  CXX_STANDARD 20
  CXX_STANDARD_REQUIRED ON
)

set_target_properties(unit_size_tests PROPERTIES
  CXX_STANDARD 20
  CXX_STANDARD_REQUIRED ON
)

function(add_unit_test_with_runtime_path test_name target_name)
  # Use the resolved executable path to avoid relying on test working directory.
  add_test(NAME ${test_name} COMMAND $<TARGET_FILE:${target_name}>)

  # When BUILD_SHARED_LIBS is ON on Windows, unit tests need fifechan.dll on PATH.
  if(WIN32)
    set_tests_properties(${test_name} PROPERTIES
      ENVIRONMENT "PATH=$<TARGET_FILE_DIR:${PROJECT_NAME}>;$ENV{PATH}"
    )
  endif()
endfunction()

add_unit_test_with_runtime_path(unit_color_tests unit_color_tests)
add_unit_test_with_runtime_path(unit_size_tests unit_size_tests)
