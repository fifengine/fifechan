message(STATUS "---------------------------------------------------------------")
message(STATUS "[ 🛠️ ] Building Unit Tests")
message(STATUS "---------------------------------------------------------------")

find_package(Catch2 CONFIG REQUIRED)

message(STATUS "All tests are linked into a single executable `all_tests`.")

set(UNIT_TEST_SOURCES
  src/color.tests.cpp
  src/size.tests.cpp
  src/rectangle.tests.cpp
  src/utf8stringeditor.tests.cpp
  src/widgets/textbox.tests.cpp
)

# Skip some tests in CI env.
# https://docs.github.com/en/actions/reference/workflows-and-actions/variables
if(NOT (DEFINED ENV{CI} OR DEFINED ENV{GITHUB_ACTIONS}))
  list(APPEND UNIT_TEST_SOURCES
    src/backends/sdl2/image.tests.cpp
    src/backends/sdl2/imagefont.tests.cpp
  )
else()
  message(STATUS "CI detected: some tests are skipped as they require a non-headless environment.")
endif()

add_executable(all_tests ${UNIT_TEST_SOURCES})

target_link_libraries(all_tests PRIVATE
  Catch2::Catch2WithMain
  # link the object library
  fifechan_lib
  # link SDL2 for backend tests
  $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
  $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>
  # link SDL2 backend library for Image class
  fifechan_sdl2
)

# Copy test resources to the build directory
set(RESOURCES_SRC_DIR "${CMAKE_SOURCE_DIR}/tests/resources")
set(RESOURCES_DST_DIR "${CMAKE_CURRENT_BINARY_DIR}/tests/resources")

if(NOT EXISTS "${RESOURCES_DST_DIR}")
  file(MAKE_DIRECTORY "${RESOURCES_DST_DIR}")
endif()

file(GLOB RESOURCE_FILES "${RESOURCES_SRC_DIR}/*")
foreach(file IN LISTS RESOURCE_FILES)
  get_filename_component(filename ${file} NAME)
  if(NOT EXISTS "${RESOURCES_DST_DIR}/${filename}")
    file(COPY "${file}" DESTINATION "${RESOURCES_DST_DIR}")
  endif()
endforeach()

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
