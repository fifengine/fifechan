message(STATUS "---------------------------------------------------------------")
message(STATUS "[ 🛠️ ] Building Integration Tests")
message(STATUS "---------------------------------------------------------------")

if(TARGET fifechan_sdl3)
  add_subdirectory(sdl)
  add_subdirectory(sdlmdedit)
endif()

if(ENABLE_OPENGL AND OPENGL_FOUND AND ENABLE_SDL AND SDL3_FOUND)
  if(TARGET fifechan_sdl3 AND TARGET fifechan_opengl)
    add_subdirectory(opengl-sdl)
  endif()
endif()
