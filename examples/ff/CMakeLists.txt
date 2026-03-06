message(STATUS "---------------------------------------------------------------")
message(STATUS "[ 🛠️ ] Building example: ff")
message(STATUS "---------------------------------------------------------------")

if(ENABLE_SDL AND SDL2_FOUND)
  find_package(SDL2_mixer CONFIG REQUIRED)

  add_executable(ff_demo
    src/ffcharacterchooser.cpp
    src/ffcontainer.cpp
    src/ffdemo.cpp
    src/fflistbox.cpp
    src/ffscrollarea.cpp
    src/guichanffdemo.cpp
  )

  target_include_directories(ff_demo PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
  )

  target_link_libraries(ff_demo PRIVATE
    $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
    $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
    $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>
    $<IF:$<TARGET_EXISTS:SDL2_mixer::SDL2_mixer>,SDL2_mixer::SDL2_mixer,SDL2_mixer::SDL2_mixer-static>
    fifechan::fifechan
    fifechan::fifechan_sdl2
  )

  set_target_properties(ff_demo PROPERTIES
    CXX_STANDARD           20
    CXX_STANDARD_REQUIRED  ON
  )

  show_build_target_properties(ff_demo)

  add_custom_command(
    TARGET ff_demo POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --cyan "Copying ff resources for 'ff_demo'"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
      "${CMAKE_CURRENT_SOURCE_DIR}/images"
      "$<TARGET_FILE_DIR:ff_demo>/images"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
      "${CMAKE_CURRENT_SOURCE_DIR}/sound"
      "$<TARGET_FILE_DIR:ff_demo>/sound"
    VERBATIM
  )

  fifechan_install_example(ff
    TARGETS ff_demo
    DIRECTORIES
      "${CMAKE_CURRENT_SOURCE_DIR}/images"
      "${CMAKE_CURRENT_SOURCE_DIR}/sound"
  )
else()
  message(STATUS "[INFO] Skipping example 'ff': requires ENABLE_SDL+SDL2_FOUND.")
endif()
