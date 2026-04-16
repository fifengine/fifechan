message(STATUS "---------------------------------------------------------------")
message(STATUS "[ 🛠️ ] Building Unit Tests")
message(STATUS "---------------------------------------------------------------")

find_package(Catch2 CONFIG REQUIRED)

add_executable(all_tests
  src/color.tests.cpp
  src/size.tests.cpp
  src/rectangle.tests.cpp
  src/utf8stringeditor.tests.cpp
  src/widgets/textbox.tests.cpp
)

target_link_libraries(all_tests PRIVATE
  Catch2::Catch2WithMain
  -Wl,--whole-archive ${PROJECT_NAME} -Wl,--no-whole-archive
)

set_target_properties(all_tests PROPERTIES
  CXX_STANDARD 20
  CXX_STANDARD_REQUIRED ON
)

function(add_unit_test_with_runtime_path test_name target_name)
  add_test(NAME ${test_name} COMMAND $<TARGET_FILE:${target_name}>)

  if(WIN32)
    set_tests_properties(${test_name} PROPERTIES
      ENVIRONMENT "PATH=$<TARGET_FILE_DIR:${PROJECT_NAME}>;$ENV{PATH}"
    )
  endif()
endfunction()

add_unit_test_with_runtime_path(all_tests all_tests)
