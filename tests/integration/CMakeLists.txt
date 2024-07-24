message(STATUS "---------------------------------------------------------------")
message(STATUS "[ 🛠️ ] Building Integration Tests")
message(STATUS "---------------------------------------------------------------")

if(ENABLE_SDL AND SDL2_FOUND)
  add_subdirectory(sdl)
endif()

#if(ENABLE_OPENGL AND OPENGL_FOUND AND ENABLE_SDL AND SDL2_FOUND)
#  add_subdirectory(opengl-sdl)
#endif()
