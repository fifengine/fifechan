#message(STATUS "---------------------------------------------------------------")
#message(STATUS "[ 🛠️ ] Building integration tests for SDL2")
#message(STATUS "---------------------------------------------------------------")

#------------------------------------------------------------------------------
# sdlhelloworld
#------------------------------------------------------------------------------

add_executable(sdlhelloworld sdlhelloworld.cpp)

target_include_directories(sdlhelloworld PRIVATE
  ${SDL_INCLUDE_DIR}
  ${SDL_IMAGE_INCLUDE_DIR}
  ${FIFECHAN_INCLUDE_DIR}
)

target_link_libraries(sdlhelloworld PRIVATE
  # SDL2
  $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
  $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
  ${SDL_IMAGE_LIBRARIES}
  fifechan_sdl
  fifechan
)

# Define the source and destination paths
set(IMAGE_SOURCE      "${PROJECT_SOURCE_DIR}/tests/resources/fixedfont.bmp")
set(IMAGE_DESTINATION "$<TARGET_FILE_DIR:sdlhelloworld>/fixedfont.bmp")
# Add a custom command to copy the image file to the build directory
add_custom_command(
    TARGET sdlhelloworld POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${IMAGE_SOURCE}"
        "${IMAGE_DESTINATION}"
    COMMENT "Copying image file to build directory"
)
# Add a custom target to ensure the image file is copied before the executable is built
add_custom_target(copy_resources ALL DEPENDS ${IMAGE_DESTINATION})
add_dependencies(sdlhelloworld copy_resources)

install(FILES "${IMAGE_SOURCE}" DESTINATION "${CMAKE_INSTALL_BINDIR}")

#------------------------------------------------------------------------------
# sdlwidgets
#------------------------------------------------------------------------------

add_executable(sdlwidgets sdlwidgets.cpp)

target_include_directories(sdlwidgets PRIVATE
  ${SDL_INCLUDE_DIR}
  ${SDL_IMAGE_INCLUDE_DIR}
  ${FIFECHAN_INCLUDE_DIR}
)

target_link_libraries(sdlwidgets PRIVATE
  # SDL2
  $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
  $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
  # SDL2_image
  ${SDL_IMAGE_LIBRARIES}
  fifechan_sdl
  fifechan
)

