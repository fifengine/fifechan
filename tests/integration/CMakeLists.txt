message(STATUS "---------------------------------------------------------------")
message(STATUS "[ 🛠️ ] Building Integration Tests")
message(STATUS "---------------------------------------------------------------")

if(ENABLE_SDL AND SDL2_FOUND)
  add_subdirectory(sdl)
endif()

#if(ENABLE_OPENGL AND OPENGL_FOUND AND ENABLE_SDL AND SDL2_FOUND)
#  add_subdirectory(opengl-sdl)
#endif()

if(ENABLE_ALLEGRO AND ALLEGRO_FOUND)
  add_subdirectory(allegro)
endif()

if(ENABLE_OPENGL AND OPENGL_FOUND AND ENABLE_ALLEGRO AND ALLEGRO_FOUND)
  add_subdirectory(opengl-allegro)
endif()

#if(ENABLE_IRRLICHT AND IRRLICHT_FOUND)
#  add_subdirectory(irrlicht)
#endif()
