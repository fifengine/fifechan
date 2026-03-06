message(STATUS "---------------------------------------------------------------")
message(STATUS "[ 🛠️ ] Building example: fps")
message(STATUS "---------------------------------------------------------------")

if(ENABLE_SDL AND SDL2_FOUND AND ENABLE_OPENGL AND OPENGL_FOUND AND OPENGL_glu_LIBRARY)
  find_package(SDL2_mixer CONFIG REQUIRED)
  find_package(PNG REQUIRED)

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

  # find_package(OpenGL COMPONENTS GLU)
  if(TARGET OpenGL::GLU)
    target_link_libraries(fps_demo PRIVATE OpenGL::GLU)
  endif()

  target_compile_features(fps_demo PRIVATE cxx_std_20)

  show_build_target_properties(fps_demo)

  # Resource Copying
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

  # Windows DLL Handling
  if(WIN32 AND BUILD_SHARED_LIBS)
    set(FPS_RUNTIME_TARGETS
      Freetype::Freetype
    )

    set(FPS_PNG_RUNTIME_TARGET
      $<IF:$<TARGET_EXISTS:PNG::PNG>,PNG::PNG,PNG::png_shared>
    )

    list(APPEND FPS_RUNTIME_TARGETS ${FPS_PNG_RUNTIME_TARGET})

    # Optional Dependencies (add if target exists)
    set(_optional_runtime_deps
      BZip2::BZip2
      unofficial::brotli::brotlidec
      unofficial::brotli::brotlicommon
    )

    foreach(dep ${_optional_runtime_deps})
      if(TARGET ${dep})
        list(APPEND FPS_RUNTIME_TARGETS ${dep})
      endif()
    endforeach()

    set(FPS_RUNTIME_DLLS
      $<TARGET_FILE:Freetype::Freetype>
      $<TARGET_FILE:${FPS_PNG_RUNTIME_TARGET}>
      $<$<TARGET_EXISTS:BZip2::BZip2>:$<TARGET_FILE:BZip2::BZip2>>
      $<$<TARGET_EXISTS:unofficial::brotli::brotlidec>:$<TARGET_FILE:unofficial::brotli::brotlidec>>
      $<$<TARGET_EXISTS:unofficial::brotli::brotlicommon>:$<TARGET_FILE:unofficial::brotli::brotlicommon>>
    )

    # Copy DLLs
    add_custom_command(
      TARGET fps_demo POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --cyan "Copying fps runtime DLLs for 'fps_demo'"
      COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${FPS_RUNTIME_DLLS}
        $<TARGET_FILE_DIR:fps_demo>
      COMMAND_EXPAND_LISTS
      VERBATIM
    )

    # Install Runtime Artifacts
    install(IMPORTED_RUNTIME_ARTIFACTS ${FPS_RUNTIME_TARGETS}
      RUNTIME DESTINATION "${FIFECHAN_EXAMPLE_INSTALL_BASE}/fps"
    )
  endif()

  fifechan_install_example(fps
    TARGETS fps_demo
    DIRECTORIES
      "${CMAKE_CURRENT_SOURCE_DIR}/images"
      "${CMAKE_CURRENT_SOURCE_DIR}/sound"
  )
else()
  message(STATUS "[INFO] Skipping example 'fps': requires ENABLE_SDL+SDL2_FOUND, ENABLE_OPENGL+OPENGL_FOUND, and GLU (OPENGL_glu_LIBRARY).")
endif()
