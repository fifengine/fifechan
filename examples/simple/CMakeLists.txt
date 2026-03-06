message(STATUS "---------------------------------------------------------------")
message(STATUS "[ 🛠️ ] Building example: simple")
message(STATUS "---------------------------------------------------------------")

if(ENABLE_SDL AND SDL2_FOUND AND ENABLE_OPENGL AND OPENGL_FOUND)
  add_executable(simple_textbox_demo simple_textbox_demo.cpp)

  target_link_libraries(simple_textbox_demo PRIVATE
    $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
    $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
    $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>
    fifechan::fifechan
    fifechan::fifechan_sdl2
    fifechan::fifechan_opengl
  )

  set_target_properties(simple_textbox_demo PROPERTIES
    CXX_STANDARD           20
    CXX_STANDARD_REQUIRED  ON
  )

  show_build_target_properties(simple_textbox_demo)

  set(FIXEDFONT_SOURCE "${PROJECT_SOURCE_DIR}/tests/resources/rpgfont.png")
  set(FIXEDFONT_DESTINATION "$<TARGET_FILE_DIR:simple_textbox_demo>/rpgfont.png")

  copy_resources(simple_textbox_demo "${FIXEDFONT_SOURCE}" "${FIXEDFONT_DESTINATION}")
  fifechan_install_example(simple
    TARGETS simple_textbox_demo
    FILES "${FIXEDFONT_SOURCE}"
  )
else()
  message(STATUS "[INFO] Skipping example 'simple': requires ENABLE_SDL+SDL2_FOUND and ENABLE_OPENGL+OPENGL_FOUND.")
endif()
