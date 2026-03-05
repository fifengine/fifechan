message(STATUS "---------------------------------------------------------------")
message(STATUS "[ 🛠️ ] Building example: fps")
message(STATUS "---------------------------------------------------------------")

if(ENABLE_SDL AND SDL2_FOUND AND ENABLE_OPENGL AND OPENGL_FOUND AND OPENGL_glu_LIBRARY)
  find_package(SDL2_mixer CONFIG REQUIRED)

  add_executable(fps_demo
    src/fifechanfpsdemo.cpp
    src/fpsbutton.cpp
    src/fpscheckbox.cpp
    src/fpsdemo.cpp
  )

  target_include_directories(fps_demo PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
  )

  target_link_libraries(fps_demo PRIVATE
    $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
    $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
    $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>
    $<IF:$<TARGET_EXISTS:SDL2_mixer::SDL2_mixer>,SDL2_mixer::SDL2_mixer,SDL2_mixer::SDL2_mixer-static>
    fifechan::fifechan
    fifechan::fifechan_sdl2
    fifechan::fifechan_opengl
  )

  if(TARGET OpenGL::GL)
    target_link_libraries(fps_demo PRIVATE OpenGL::GL)
  endif()

  if(TARGET OpenGL::GLU)
    target_link_libraries(fps_demo PRIVATE OpenGL::GLU)
  elseif(OPENGL_glu_LIBRARY)
    target_link_libraries(fps_demo PRIVATE ${OPENGL_glu_LIBRARY})
  endif()

  set_target_properties(fps_demo PROPERTIES
    CXX_STANDARD           20
    CXX_STANDARD_REQUIRED  ON
  )

  show_build_target_properties(fps_demo)

  add_custom_command(
    TARGET fps_demo POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --cyan "Copying fps resources for 'fps_demo'"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
      "${CMAKE_CURRENT_SOURCE_DIR}/images"
      "$<TARGET_FILE_DIR:fps_demo>/images"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
      "${CMAKE_CURRENT_SOURCE_DIR}/sound"
      "$<TARGET_FILE_DIR:fps_demo>/sound"
    VERBATIM
  )

  install(TARGETS fps_demo
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  )

  install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/images"
    DESTINATION ${CMAKE_INSTALL_BINDIR}
  )

  install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/sound"
    DESTINATION ${CMAKE_INSTALL_BINDIR}
  )
else()
  message(STATUS "[INFO] Skipping example 'fps': requires ENABLE_SDL+SDL2_FOUND, ENABLE_OPENGL+OPENGL_FOUND, and GLU (OPENGL_glu_LIBRARY).")
endif()
