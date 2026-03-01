#message(STATUS "---------------------------------------------------------------")
#message(STATUS "[ 🛠️ ] Building integration tests for SDL2")
#message(STATUS "---------------------------------------------------------------")

set(FIFECHAN_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include")

#------------------------------------------------------------------------------
# sdlhelloworld
#------------------------------------------------------------------------------

add_executable(sdlhelloworld sdlhelloworld.cpp)

target_include_directories(sdlhelloworld PRIVATE
  ${SDL2_INCLUDE_DIRS}
  ${FIFECHAN_INCLUDE_DIR}
)

target_link_libraries(sdlhelloworld PRIVATE
  # SDL2
  $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
  $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
  # SDL2_image
  $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>
  # Fifechan
  fifechan::fifechan
  # Fifechan SDL extension
  fifechan::fifechan_sdl
)

set_target_properties(sdlhelloworld PROPERTIES
  CXX_STANDARD           20
  CXX_STANDARD_REQUIRED  ON
)

show_build_target_properties(sdlhelloworld)

# Define the source and destination paths
set(IMAGE_SOURCE      "${PROJECT_SOURCE_DIR}/tests/resources/fixedfont.bmp")
set(IMAGE_DESTINATION "$<TARGET_FILE_DIR:sdlhelloworld>/fixedfont.bmp")
copy_resources(sdlhelloworld "${IMAGE_SOURCE}" "${IMAGE_DESTINATION}")
install(FILES "${IMAGE_SOURCE}" DESTINATION "${CMAKE_INSTALL_BINDIR}")

#------------------------------------------------------------------------------
# sdlwidgets
#------------------------------------------------------------------------------

add_executable(sdlwidgets sdlwidgets.cpp)

target_include_directories(sdlwidgets PRIVATE
  ${SDL2_INCLUDE_DIR}
  ${FIFECHAN_INCLUDE_DIR}
)

target_link_libraries(sdlwidgets PRIVATE
  # SDL2
  $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
  $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
  # SDL2_image
  $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>
  # Fifechan
  fifechan::fifechan
  # Fifechan SDL extension
  fifechan::fifechan_sdl
)

set_target_properties(sdlwidgets PROPERTIES
  CXX_STANDARD           20
  CXX_STANDARD_REQUIRED  ON
)

show_build_target_properties(sdlwidgets)
