message(STATUS "---------------------------------------------------------------")
message(STATUS "[ 🛠️ ] Building Integration Test: sdlmdedit")
message(STATUS "---------------------------------------------------------------")

if(ENABLE_SDL AND SDL2_FOUND)
  add_executable(sdlmdedit main.cpp)

  target_link_libraries(sdlmdedit PRIVATE
    $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
    $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
    $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>
    fifechan::fifechan
    fifechan::fifechan_sdl2
  )

  set_target_properties(sdlmdedit PROPERTIES
    CXX_STANDARD           20
    CXX_STANDARD_REQUIRED  ON
  )

  show_build_target_properties(sdlmdedit)

else()
  message(STATUS "[INFO] Skipping integration test 'sdlmdedit': requires ENABLE_SDL+SDL2_FOUND.")
endif()