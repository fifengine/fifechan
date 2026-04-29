message(STATUS "---------------------------------------------------------------")
message(STATUS "[ 🛠️ ] Building Integration Test: sdlmdedit")
message(STATUS "---------------------------------------------------------------")

if(ENABLE_SDL AND SDL3_FOUND)
  add_executable(sdlmdedit main.cpp)

  target_link_libraries(sdlmdedit PRIVATE
    $<IF:$<TARGET_EXISTS:SDL3::SDL3>,SDL3::SDL3,SDL3::SDL3-static>
    $<IF:$<TARGET_EXISTS:SDL3_image::SDL3_image>,SDL3_image::SDL3_image,SDL3_image::SDL3_image-static>
    fifechan::fifechan
    fifechan::fifechan_sdl3
  )

  set_target_properties(sdlmdedit PROPERTIES
    CXX_STANDARD           20
    CXX_STANDARD_REQUIRED  ON
  )

  show_build_target_properties(sdlmdedit)

else()
  message(STATUS "[INFO] Skipping integration test 'sdlmdedit': requires ENABLE_SDL+SDL3_FOUND.")
endif()
