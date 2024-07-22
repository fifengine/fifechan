

add_executable(allegrohelloworld allegrohelloworld.cpp)
target_link_libraries(allegrohelloworld PRIVATE
    ${ALLEGRO_LIBRARIES}
    fifechan_allegro
    fifechan
)

add_executable(allegrowidgets allegrowidgets.cpp)
target_link_libraries(allegrowidgets PRIVATE
    ${ALLEGRO_LIBRARIES}
    fifechan_allegro
    fifechan
)

add_executable(allegroaction allegroaction.cpp)
target_link_libraries(allegroaction PRIVATE
    ${ALLEGRO_LIBRARIES}
    fifechan_allegro
    fifechan
)
