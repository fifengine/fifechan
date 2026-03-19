message(STATUS "---------------------------------------------------------------")
message(STATUS "[ 🛠️ ] Building Integration Tests for OpenGL+SDL2")
message(STATUS "---------------------------------------------------------------")

set(FIFEGUI_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include")
set(FIFEGUI_VERSION_FILE_DIR "${CMAKE_CURRENT_BINARY_DIR}/../../../include")

if(DEFINED ENV{VCPKG_ROOT})
  include("$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
endif()

#------------------------------------------------------------------------------
# openglsdlhelloworld
#------------------------------------------------------------------------------

add_executable(openglsdlhelloworld openglsdlhelloworld.cpp openglsdlhelloworld.hpp)

target_include_directories(openglsdlhelloworld PRIVATE
  ${SDL2_INCLUDE_DIRS}
  ${OPENGL_INCLUDE_DIR}
  ${FIFEGUI_INCLUDE_DIR}
  ${FIFEGUI_VERSION_FILE_DIR}
  ${PROJECT_SOURCE_DIR}/include/fifechan/backends/sdl2
  ${PROJECT_SOURCE_DIR}/include/fifechan/backends/opengl
)

target_link_libraries(openglsdlhelloworld PRIVATE
  # SDL2
  $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
  $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
  # SDL2_image
  $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>
  # OpenGL
  OpenGL::GL
  # FifeGUI
  fifechan::fifechan
  # FifeGUI SDL extension
  fifechan::fifechan_sdl2
  # FifeGUI OpenGL extension
  fifechan::fifechan_opengl
)

set_target_properties(openglsdlhelloworld PROPERTIES
  CXX_STANDARD           20
  CXX_STANDARD_REQUIRED  ON
)

if(WIN32)
  set_target_properties(openglsdlhelloworld PROPERTIES
    WIN32_EXECUTABLE TRUE
  )
endif()

if(BUILD_SHARED_LIBS)
  target_compile_definitions(openglsdlhelloworld PRIVATE "FIFEGUI_DLL_IMPORT")
else()
  target_compile_definitions(openglsdlhelloworld INTERFACE "FIFEGUI_STATIC;FIFEGUI_EXT_STATIC")
endif()

show_build_target_properties(openglsdlhelloworld)

set(RESOURCES_FOLDER "${PROJECT_SOURCE_DIR}/tests/resources")
copy_resources_folder(openglsdlhelloworld "${RESOURCES_FOLDER}")

#------------------------------------------------------------------------------
# openglsdlwidgets
#------------------------------------------------------------------------------

add_executable(openglsdlwidgets openglsdlwidgets.cpp openglsdlwidgets.hpp)

target_include_directories(openglsdlwidgets PRIVATE
  ${SDL2_INCLUDE_DIRS}
  ${OPENGL_INCLUDE_DIR}
  ${FIFEGUI_INCLUDE_DIR}
  ${FIFEGUI_VERSION_FILE_DIR}
  ${PROJECT_SOURCE_DIR}/include/fifechan/backends/sdl2
  ${PROJECT_SOURCE_DIR}/include/fifechan/backends/opengl
)

target_link_libraries(openglsdlwidgets PRIVATE
  # SDL2
  $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
  $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
  # SDL2_image
  $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>
  # OpenGL
  OpenGL::GL
  # FifeGUI
  fifechan::fifechan
  # FifeGUI SDL extension
  fifechan::fifechan_sdl2
  # FifeGUI OpenGL extension
  fifechan::fifechan_opengl
)

set_target_properties(openglsdlwidgets PROPERTIES
  CXX_STANDARD           20
  CXX_STANDARD_REQUIRED  ON
)

if(WIN32)
  set_target_properties(openglsdlwidgets PROPERTIES
    WIN32_EXECUTABLE TRUE
  )
endif()

if(BUILD_SHARED_LIBS)
  target_compile_definitions(openglsdlwidgets PRIVATE "FIFEGUI_DLL_IMPORT")
else()
  target_compile_definitions(openglsdlwidgets INTERFACE "FIFEGUI_STATIC;FIFEGUI_EXT_STATIC")
endif()

show_build_target_properties(openglsdlwidgets)

copy_resources_folder(openglsdlwidgets "${RESOURCES_FOLDER}")
