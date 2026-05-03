message(STATUS "---------------------------------------------------------------")
message(STATUS "[ 🛠️ ] Building example: spacers")
message(STATUS "---------------------------------------------------------------")

if(ENABLE_SDL AND SDL3_FOUND)
  if(WIN32 AND BUILD_SHARED_LIBS)
    set(SPACERS_WINDOWS_RUNTIME_FILES
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
      "$<$<CONFIG:Debug>:${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/debug/bin/zd.dll>"
      "$<$<NOT:$<CONFIG:Debug>>:${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/bin/z.dll>"
    )
  endif()

  add_executable(spacers_in_vbox_hbox spacers_in_vbox_hbox.cpp)
  add_executable(dynamic_remove_spacers dynamic_remove_spacers.cpp)

  target_link_libraries(spacers_in_vbox_hbox PRIVATE
    $<IF:$<TARGET_EXISTS:SDL3::SDL3>,SDL3::SDL3,SDL3::SDL3-static>
    $<IF:$<TARGET_EXISTS:SDL3_image::SDL3_image>,SDL3_image::SDL3_image,SDL3_image::SDL3_image-static>
    fifechan::fifechan
    fifechan::fifechan_sdl3
  )

  target_link_libraries(dynamic_remove_spacers PRIVATE
    $<IF:$<TARGET_EXISTS:SDL3::SDL3>,SDL3::SDL3,SDL3::SDL3-static>
    $<IF:$<TARGET_EXISTS:SDL3_image::SDL3_image>,SDL3_image::SDL3_image,SDL3_image::SDL3_image-static>
    fifechan::fifechan
    fifechan::fifechan_sdl3
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

  if(WIN32 AND BUILD_SHARED_LIBS)
    fifechan_install_example(spacers
      TARGETS
        spacers_in_vbox_hbox
        dynamic_remove_spacers
      FILES "${RPGFONT_SOURCE}"
      WINDOWS_RUNTIME_FILES ${SPACERS_WINDOWS_RUNTIME_FILES}
    )
  else()
    fifechan_install_example(spacers
      TARGETS
        spacers_in_vbox_hbox
        dynamic_remove_spacers
      FILES "${RPGFONT_SOURCE}"
    )
  endif()
else()
  message(STATUS "[INFO] Skipping example 'spacers': requires ENABLE_SDL+SDL3_FOUND.")
endif()
