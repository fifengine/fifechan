message(STATUS "---------------------------------------------------------------")
message(STATUS "[ 🛠️ ] Building example: fps")
message(STATUS "---------------------------------------------------------------")

if(ENABLE_SDL AND SDL2_FOUND AND ENABLE_OPENGL AND OPENGL_FOUND AND OPENGL_glu_LIBRARY)
  find_package(SDL2_mixer CONFIG REQUIRED)
  find_package(PNG REQUIRED)

  if(WIN32 AND BUILD_SHARED_LIBS)
    set(FPS_WINDOWS_RUNTIME_FILES
      "$<$<CONFIG:Debug>:${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/debug/bin/freetyped.dll>"
      "$<$<NOT:$<CONFIG:Debug>>:${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/bin/freetype.dll>"
      "$<$<CONFIG:Debug>:${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/debug/bin/libpng16d.dll>"
      "$<$<NOT:$<CONFIG:Debug>>:${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/bin/libpng16.dll>"
      "$<$<CONFIG:Debug>:${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/debug/bin/brotlidec.dll>"
      "$<$<NOT:$<CONFIG:Debug>>:${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/bin/brotlidec.dll>"
      "$<$<CONFIG:Debug>:${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/debug/bin/brotlicommon.dll>"
      "$<$<NOT:$<CONFIG:Debug>>:${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/bin/brotlicommon.dll>"
      "$<$<CONFIG:Debug>:${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/debug/bin/bz2d.dll>"
      "$<$<NOT:$<CONFIG:Debug>>:${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/bin/bz2.dll>"
      "$<$<CONFIG:Debug>:${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/debug/bin/zlibd1.dll>"
      "$<$<NOT:$<CONFIG:Debug>>:${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/bin/zlib1.dll>"
    )
  endif()

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

  if(WIN32)
    set_target_properties(fps_demo PROPERTIES
      WIN32_EXECUTABLE TRUE
    )
  endif()

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

  if(WIN32 AND BUILD_SHARED_LIBS)
    fifechan_install_example(fps
      TARGETS fps_demo
      DIRECTORIES
        "${CMAKE_CURRENT_SOURCE_DIR}/images"
        "${CMAKE_CURRENT_SOURCE_DIR}/sound"
      WINDOWS_RUNTIME_FILES ${FPS_WINDOWS_RUNTIME_FILES}
    )
  else()
    fifechan_install_example(fps
      TARGETS fps_demo
      DIRECTORIES
        "${CMAKE_CURRENT_SOURCE_DIR}/images"
        "${CMAKE_CURRENT_SOURCE_DIR}/sound"
    )
  endif()

else()
  message(STATUS "[INFO] Skipping example 'fps': requires ENABLE_SDL+SDL2_FOUND, ENABLE_OPENGL+OPENGL_FOUND, and GLU (OPENGL_glu_LIBRARY).")
endif()
