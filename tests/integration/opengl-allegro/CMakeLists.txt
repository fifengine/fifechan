add_executable(opengl-allegrohelloworld openglallegrohelloworld.cpp)
target_link_libraries(openglallegrohelloworld PRIVATE
    ${ALLEGRO_LIBRARIES}
    ${GL_LIBRARIES}
    fifechan_opengl
    fifechan_allegro
    fifechan
)

add_executable(opengl-allegrowidgets openglallegrowidgets.cpp)
target_link_libraries(openglallegrowidgets PRIVATE
    ${ALLEGRO_LIBRARIES}
    ${GL_LIBRARIES}
    fifechan_opengl
    fifechan_allegro
    fifechan
)

add_executable(opengl-allegroaction openglallegroaction.cpp)
target_link_libraries(openglallegroaction PRIVATE
    ${ALLEGRO_LIBRARIES}
    ${GL_LIBRARIES}
    fifechan_opengl
    fifechan_allegro
    fifechan
)
