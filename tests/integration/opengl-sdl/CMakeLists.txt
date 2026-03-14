project(tests-integration-opengl-sdl)


#------------------------------------------------------------------------------
# openglsdlhelloworld
#------------------------------------------------------------------------------

add_executable(openglsdlhelloworld openglsdlhelloworld.cpp openglsdlhelloworld.hpp)
target_include_directories(openglsdlhelloworld PRIVATE
    ${SDL_INCLUDE_DIRS}
    ${SDL_IMAGE_INCLUDE_DIRS}
    ${GL_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/include
)
target_link_libraries(openglsdlhelloworld PRIVATE
    # SDL2
    $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
    $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
    # SDL2_image
    $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>
    # OpenGL
    ${GL_LIBRARIES}
    fifechan_sdl2
    fifechan_opengl
    fifechan
)
show_build_target_properties(openglsdlhelloworld)

if(WIN32)
    set_target_properties(openglsdlhelloworld PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
endif()

#------------------------------------------------------------------------------
# openglsdlwidgets
#------------------------------------------------------------------------------

add_executable(openglsdlwidgets openglsdlwidgets.cpp openglsdlwidgets.hpp)
target_link_libraries(openglsdlwidgets PRIVATE
    ${SDL_LIBRARIES}
    ${SDL_IMAGE_LIBRARIES}
    ${GL_LIBRARIES}
    fifechan_sdl2
    fifechan_opengl
    fifechan
)
show_build_target_properties(openglsdlwidgets)

if(WIN32)
    set_target_properties(openglsdlwidgets PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
endif()

#------------------------------------------------------------------------------
# openglsdlaction
#------------------------------------------------------------------------------

add_executable(openglsdlaction openglsdlaction.cpp openglsdlaction.hpp)
target_link_libraries(openglsdlaction PRIVATE
    ${SDL_LIBRARIES}
    ${SDL_IMAGE_LIBRARIES}
    ${GL_LIBRARIES}
    fifechan_sdl2
    fifechan_opengl
    fifechan
)
show_build_target_properties(openglsdlaction)

if(WIN32)
    set_target_properties(openglsdlaction PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
endif()

