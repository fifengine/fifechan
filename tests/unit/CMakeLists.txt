message(STATUS "---------------------------------------------------------------")
message(STATUS "[ 🛠️ ] Building Unit Tests")
message(STATUS "---------------------------------------------------------------")

find_package(Catch2 CONFIG REQUIRED)

message(STATUS "All tests are linked into a single executable `all_tests`.")

set(UNIT_TEST_SOURCES
  src/cliprectangle.tests.cpp
  src/font.tests.cpp
  src/graphics.tests.cpp
  src/fifechan.tests.cpp
  src/color.tests.cpp
  src/size.tests.cpp
  src/rectangle.tests.cpp
  src/genericinput.tests.cpp
  src/defaultfont.tests.cpp
  src/visibilityeventhandler.tests.cpp
  src/image.tests.cpp
  # Input and key tests
  src/key.tests.cpp
  src/keyinput.tests.cpp
  src/mouseinput.tests.cpp
  src/shortcut.tests.cpp
  # Event tests
  src/events/event.tests.cpp
  src/events/actionevent.tests.cpp
  src/events/containerevent.tests.cpp
  src/events/selectionevent.tests.cpp
  src/events/dragevent.tests.cpp
  src/events/mouseevent.tests.cpp
  # Widget tests
  src/widget.tests.cpp
  src/widgets/focus.tests.cpp
  src/focushandler.tests.cpp
  src/widgets/primarypanel.tests.cpp
  src/widgets/secondarypanel.tests.cpp
  src/widgets/spacer.tests.cpp
  src/widgets/barsection.tests.cpp
  src/widgets/bargraph.tests.cpp
  src/widgets/container.tests.cpp
  # Text Input and UTF-8 tests
  src/text.tests.cpp
  src/utf8stringeditor.tests.cpp
  # Widgets
  src/widgets/textbox.tests.cpp
  src/widgets/label.tests.cpp
)

# TODO: this can be tested with software rasterizer backend in CI.
# Skip some tests in CI env.
# https://docs.github.com/en/actions/reference/workflows-and-actions/variables
if(NOT (DEFINED ENV{CI} OR DEFINED ENV{GITHUB_ACTIONS}))
  list(APPEND UNIT_TEST_SOURCES
    src/backends/sdl3/image.tests.cpp
    src/imagefont.tests.cpp
    src/truetypefont.tests.cpp
  )
else()
  message(STATUS "CI detected: some tests are skipped as they require a non-headless environment.")
endif()

add_executable(all_tests ${UNIT_TEST_SOURCES})

# Link the fifechan implementation.
if(MSVC)
  # on MSVC link the public library target
  target_link_libraries(all_tests PRIVATE fifechan::fifechan)
else()
 # Use object files for coverage on Linux
  target_link_libraries(all_tests PRIVATE $<TARGET_OBJECTS:fifechan_lib>)
endif()

target_link_libraries(all_tests PRIVATE
  Catch2::Catch2WithMain
  $<IF:$<TARGET_EXISTS:SDL3::SDL3>,SDL3::SDL3,SDL3::SDL3-static>
  $<IF:$<TARGET_EXISTS:SDL3_image::SDL3_image>,SDL3_image::SDL3_image,SDL3_image::SDL3_image-static>
  $<IF:$<TARGET_EXISTS:SDL3_ttf::SDL3_ttf>,SDL3_ttf::SDL3_ttf,SDL3_ttf::SDL3_ttf-static>
  # link SDL3 backend library for Image class
  fifechan_sdl3
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
