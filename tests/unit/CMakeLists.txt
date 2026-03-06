message(STATUS "---------------------------------------------------------------")
message(STATUS "[ 🛠️ ] Building Unit Tests")
message(STATUS "---------------------------------------------------------------")

find_package(Catch2 CONFIG REQUIRED)

add_executable(unit_color_tests
  src/color_test.cpp
)

add_executable(unit_size_tests
  src/size_test.cpp
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

add_test(NAME unit_color_tests COMMAND unit_color_tests)
add_test(NAME unit_size_tests COMMAND unit_size_tests)
