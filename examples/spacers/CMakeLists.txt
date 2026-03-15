message(STATUS "---------------------------------------------------------------")
message(STATUS "[ 🛠️ ] Building example: spacers")
message(STATUS "---------------------------------------------------------------")

if(ENABLE_SDL AND SDL2_FOUND)
  add_executable(spacers_in_vbox_hbox spacers_in_vbox_hbox.cpp)
  add_executable(dynamic_remove_spacers dynamic_remove_spacers.cpp)

  target_link_libraries(spacers_in_vbox_hbox PRIVATE
    $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
    $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
    $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>
    fifechan::fifechan
    fifechan::fifechan_sdl2
  )

  target_link_libraries(dynamic_remove_spacers PRIVATE
    $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
    $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
    $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>
    fifechan::fifechan
    fifechan::fifechan_sdl2
  )

  set_target_properties(spacers_in_vbox_hbox dynamic_remove_spacers PROPERTIES
    CXX_STANDARD           20
    CXX_STANDARD_REQUIRED  ON
  )

  if(WIN32)
    set_target_properties(spacers_in_vbox_hbox dynamic_remove_spacers PROPERTIES
      WIN32_EXECUTABLE TRUE
    )
  endif()

  show_build_target_properties(spacers_in_vbox_hbox)
  show_build_target_properties(dynamic_remove_spacers)

  set(RPGFONT_SOURCE "${PROJECT_SOURCE_DIR}/tests/resources/rpgfont.png")
  set(SPACERS_IN_VBOX_HBOX_RPGFONT_DESTINATION "$<TARGET_FILE_DIR:spacers_in_vbox_hbox>/rpgfont.png")
  set(DYNAMIC_REMOVE_SPACERS_RPGFONT_DESTINATION "$<TARGET_FILE_DIR:dynamic_remove_spacers>/rpgfont.png")

  copy_resources(spacers_in_vbox_hbox "${RPGFONT_SOURCE}" "${SPACERS_IN_VBOX_HBOX_RPGFONT_DESTINATION}")
  copy_resources(dynamic_remove_spacers "${RPGFONT_SOURCE}" "${DYNAMIC_REMOVE_SPACERS_RPGFONT_DESTINATION}")
  fifechan_install_example(spacers
    TARGETS
      spacers_in_vbox_hbox
      dynamic_remove_spacers
    FILES "${RPGFONT_SOURCE}"
  )
else()
  message(STATUS "[INFO] Skipping example 'spacers': requires ENABLE_SDL+SDL2_FOUND.")
endif()
