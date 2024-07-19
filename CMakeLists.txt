#------------------------------------------------------------------------------
# FifeChan / FifeGUI
#------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.0...3.30)

#-------------------------------------------------------------------
# Project
#-------------------------------------------------------------------

set(MAJOR_VERSION 0)
set(MINOR_VERSION 1)
set(PATCH_VERSION 5)
set(FIFECHAN_VERSION ${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_VERSION})

project(fifechan VERSION ${FIFECHAN_VERSION} LANGUAGES CXX)

#------------------------------------------------------------------------------
# Options
#------------------------------------------------------------------------------

option(BUILD_SHARED_LIBS     "Build using shared libraries"                     ON)

option(ENABLE_ALLEGRO        "Enable the Fifechan Allegro extension"            OFF)
option(ENABLE_IRRLICHT       "Enable the Fifechan Irrlicht extension"           OFF)
option(ENABLE_OPENGL         "Enable the Fifechan OpenGL extension"             ON)
option(ENABLE_SDL            "Enable the Fifechan SDL extension"                ON)

#------------------------------------------------------------------------------
# Default build type
#------------------------------------------------------------------------------

# The default build type is based on BUILD_SHARED_LIBS.
# FIFECHAN_LIBRARY_BUILD_TYPE is used on add_library() calls to set it.
if(BUILD_SHARED_LIBS)
  set(FIFECHAN_LIBRARY_BUILD_TYPE SHARED)
else()
  set(FIFECHAN_LIBRARY_BUILD_TYPE STATIC)
endif()

set(LIB_SUFFIX "" CACHE STRING "Suffix to append to the system lib folder")

#------------------------------------------------------------------------------
# Display Status
#------------------------------------------------------------------------------

message("")
message("== Building FIFECHAN version ${FIFECHAN_VERSION} using build type '${CMAKE_BUILD_TYPE}'.")
message(STATUS "    Source directory is '${PROJECT_SOURCE_DIR}'.")
message(STATUS "    Build  directory is '${PROJECT_BINARY_DIR}'.")
message(STATUS "    Installation path is '${CMAKE_INSTALL_PREFIX}' (set -DCMAKE_INSTALL_PREFIX=<path> to change).")
message("")

#------------------------------------------------------------------------------
# Find Dependencies - Windows
#------------------------------------------------------------------------------

# Tell CMake where to search for binaries (dlls), includes and libs on Windows
if(WIN32)
  set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH}
    "${PROJECT_SOURCE_DIR}/../fifechan-dependencies/includes/bin"
    "${PROJECT_SOURCE_DIR}/../fifechan-dependencies/includes/include"
    "${PROJECT_SOURCE_DIR}/../fifechan-dependencies/includes/include/SDL2"
    "${PROJECT_SOURCE_DIR}/../fifechan-dependencies/includes/lib/x86"
    "${PROJECT_SOURCE_DIR}/../fifechan-dependencies/includes/lib"
  )
endif()

#------------------------------------------------------------------------------
# Configure
#------------------------------------------------------------------------------

# Find additional CMake modules
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake/finders")

# Do not allow an in-source-tree build, request an out-of-source-tree build.
if (CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
  message(FATAL_ERROR "#Please build outside of the source tree!\n
                       Make a separate folder for building and run CMake there:\n
                       mkdir _build; cd _build; cmake ..\n
                       Before that, cleanup:\nrm -rf CMakeCache.txt CMakeFiles")
endif()

# Disable in-source builds and modifications to the source tree.
set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

# Create compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#------------------------------------------------------------------------------
# Compiler Options
#------------------------------------------------------------------------------

if(WIN32)
  if(MSVC)
    # ensure we use minimal "windows.h" lib without the crazy min max macros
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /D \"WIN32_LEAN_AND_MEAN\"")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /D \"NOMINMAX\"")
  endif()
endif()

#------------------------------------------------------------------------------
# Source Files
#------------------------------------------------------------------------------

# Add the include directory to the compiler's search path
include_directories(${PROJECT_SOURCE_DIR}/include)

# The Fifechan core library
file(GLOB FIFECHAN_HEADER include/fifechan.hpp)
file(GLOB FIFECHAN_HEADERS
  include/fifechan/actionevent.hpp
  include/fifechan/actionlistener.hpp
  include/fifechan/cliprectangle.hpp
  include/fifechan/color.hpp
  include/fifechan/containerevent.hpp
  include/fifechan/containerlistener.hpp
  include/fifechan/deathlistener.hpp
  include/fifechan/defaultfont.hpp
  include/fifechan/event.hpp
  include/fifechan/exception.hpp
  include/fifechan/focushandler.hpp
  include/fifechan/focuslistener.hpp
  include/fifechan/font.hpp
  include/fifechan/genericinput.hpp
  include/fifechan/glut.hpp
  include/fifechan/graphics.hpp
  include/fifechan/gui.hpp
  include/fifechan/image.hpp
  include/fifechan/imagefont.hpp
  include/fifechan/imageloader.hpp
  include/fifechan/input.hpp
  include/fifechan/inputevent.hpp
  include/fifechan/key.hpp
  include/fifechan/keyevent.hpp
  include/fifechan/keyinput.hpp
  include/fifechan/keylistener.hpp
  include/fifechan/listmodel.hpp
  include/fifechan/mouseevent.hpp
  include/fifechan/mouseinput.hpp
  include/fifechan/mouselistener.hpp
  include/fifechan/platform.hpp
  include/fifechan/point.hpp
  include/fifechan/rectangle.hpp
  include/fifechan/selectionevent.hpp
  include/fifechan/selectionlistener.hpp
  include/fifechan/size.hpp
  include/fifechan/text.hpp
  include/fifechan/utf8stringeditor.hpp
  include/fifechan/version.hpp
  include/fifechan/visibilityeventhandler.hpp
  include/fifechan/widget.hpp
  include/fifechan/widgetlistener.hpp
  include/fifechan/opengl.hpp
)
file(GLOB FIFECHAN_WIDGET_HEADERS  include/fifechan/widgets/*.hpp)
file(GLOB FIFECHAN_UTIL_HEADERS    include/fifechan/util/*.hpp)
file(GLOB FIFECHAN_SRC             src/*.cpp)
file(GLOB FIFECHAN_WIDGET_SRC      src/widgets/*.cpp)

#------------------------------------------------------------------------------
# Fifechan core library as a shared library
#------------------------------------------------------------------------------

add_library(${PROJECT_NAME} ${FIFECHAN_LIBRARY_BUILD_TYPE}
    ${FIFECHAN_HEADER}
    ${FIFECHAN_HEADERS}
    ${FIFECHAN_WIDGET_HEADERS}
    ${FIFECHAN_UTIL_HEADERS}
    ${FIFECHAN_SRC}
    ${FIFECHAN_WIDGET_SRC}
)

set_target_properties(${PROJECT_NAME} PROPERTIES
  VERSION                ${FIFECHAN_VERSION}
  SOVERSION              ${FIFECHAN_VERSION}
  CLEAN_DIRECT_OUTPUT    1                             # Allow creating static and shared libraries without conflict
  OUTPUT_NAME            ${PROJECT_NAME}               # Avoid conflicts between library and binary target names
  COMPILE_DEFINITIONS    "FIFECHAN_BUILD"
  CXX_STANDARD           17
  CXX_STANDARD_REQUIRED  ON
)

install(TARGETS ${PROJECT_NAME} DESTINATION lib${LIB_SUFFIX} PERMISSIONS
  OWNER_READ OWNER_WRITE OWNER_EXECUTE
  GROUP_READ GROUP_EXECUTE
  WORLD_READ WORLD_EXECUTE
)

install(FILES ${FIFECHAN_HEADER}          DESTINATION include/)
install(FILES ${FIFECHAN_HEADERS}         DESTINATION include/fifechan/)
install(FILES ${FIFECHAN_WIDGET_HEADERS}  DESTINATION include/fifechan/widgets/)
install(FILES ${FIFECHAN_UTIL_HEADERS}    DESTINATION include/fifechan/util/)

#------------------------------------------------------------------------------
# The Fifechan Allegro extension library
#------------------------------------------------------------------------------

find_package(Allegro)

if(ENABLE_ALLEGRO AND ALLEGRO_FOUND)

  include_directories(${ALLEGRO_INCLUDE_DIR})

  # The Fifechan Allegro extension source
  file(GLOB FIFECHAN_ALLEGRO_HEADER include/fifechan/allegro.hpp)
  file(GLOB FIFECHAN_ALLEGRO_HEADERS include/fifechan/allegro/*.hpp)
  file(GLOB FIFECHAN_ALLEGRO_SRC src/allegro/*.cpp)

  add_library(${PROJECT_NAME}_allegro ${FIFECHAN_LIBRARY_BUILD_TYPE}
  	${FIFECHAN_ALLEGRO_HEADER}
  	${FIFECHAN_ALLEGRO_HEADERS}
  	${FIFECHAN_ALLEGRO_SRC}
  )

  target_link_libraries(${PROJECT_NAME}_allegro ${ALLEG_LIBRARY} ${PROJECT_NAME})

  set_target_properties(${PROJECT_NAME}_allegro PROPERTIES
    VERSION                ${FIFECHAN_VERSION}
    SOVERSION              ${FIFECHAN_VERSION}
    CLEAN_DIRECT_OUTPUT    1                               # Allow creating static and shared libraries without conflict
    OUTPUT_NAME            ${PROJECT_NAME}_allegro         # Avoid conflicts between library and binary target names
    COMPILE_DEFINITIONS    "FIFECHAN_EXTENSION_BUILD"
    CXX_STANDARD           17
    CXX_STANDARD_REQUIRED  ON
  )

  install(TARGETS ${PROJECT_NAME}_allegro DESTINATION lib${LIB_SUFFIX} PERMISSIONS
    OWNER_READ OWNER_WRITE OWNER_EXECUTE
    GROUP_READ GROUP_EXECUTE
    WORLD_READ WORLD_EXECUTE
  )

  install(FILES ${FIFECHAN_ALLEGRO_HEADER}   DESTINATION include/fifechan/)
  install(FILES ${FIFECHAN_ALLEGRO_HEADERS}  DESTINATION include/fifechan/allegro/)

endif()

#------------------------------------------------------------------------------
# Fifechan Irrlicht extension library
#------------------------------------------------------------------------------

find_package(Irrlicht)

if(ENABLE_IRRLICHT AND IRRLICHT_FOUND)

  include_directories(${IRRLICHT_INCLUDE_DIR})

  # The Fifechan Irrlicht extension source
  file(GLOB FIFECHAN_IRRLICHT_HEADER include/fifechan/irrlicht.hpp)
  file(GLOB FIFECHAN_IRRLICHT_HEADERS include/fifechan/irrlicht/*.hpp)
  file(GLOB FIFECHAN_IRRLICHT_SRC src/irrlicht/*.cpp)

  add_library(${PROJECT_NAME}_irrlicht ${FIFECHAN_LIBRARY_BUILD_TYPE}
    ${FIFECHAN_IRRLICHT_HEADER}
    ${FIFECHAN_IRRLICHT_HEADERS}
    ${FIFECHAN_IRRLICHT_SRC}
  )

  target_link_libraries(${PROJECT_NAME}_irrlicht ${IRRLICHT_LIBRARY} ${PROJECT_NAME})

  set_target_properties(${PROJECT_NAME}_irrlicht PROPERTIES
    VERSION                ${FIFECHAN_VERSION}
    SOVERSION              ${FIFECHAN_VERSION}
    CLEAN_DIRECT_OUTPUT    1                               # Allow creating static and shared libraries without conflict
    OUTPUT_NAME            ${PROJECT_NAME}_irrlicht        # Avoid conflicts between library and binary target names
    COMPILE_DEFINITIONS    "FIFECHAN_EXTENSION_BUILD"
    CXX_STANDARD           17
    CXX_STANDARD_REQUIRED  ON
  )

  install(TARGETS ${PROJECT_NAME}_irrlicht DESTINATION lib${LIB_SUFFIX} PERMISSIONS
    OWNER_READ OWNER_WRITE OWNER_EXECUTE
    GROUP_READ GROUP_EXECUTE
    WORLD_READ WORLD_EXECUTE
  )

  install(FILES ${FIFECHAN_IRRLICHT_HEADER}   DESTINATION include/fifechan/)
  install(FILES ${FIFECHAN_IRRLICHT_HEADERS}  DESTINATION include/fifechan/irrlicht/)

endif()

#------------------------------------------------------------------------------
# The Fifechan OpenGL extension library
#------------------------------------------------------------------------------

find_package(OpenGL)
find_package(Freetype)
find_package(OGLFT)

if(APPLE AND ENABLE_OPENGL)
  find_library(OPENGL_LIBRARY OpenGL)
  mark_as_advanced(OPENGL_LIBRARY)
  set(EXTRA_LIBS ${OPENGL_LIBRARY})
endif()

if(ENABLE_OPENGL AND OPENGL_FOUND)

  include_directories(${OPENGL_INCLUDE_DIR})
  include_directories(${FREETYPE_INCLUDE_DIRS})
  include_directories(${OGLFT_INCLUDE_DIRS})

  # The OpenGL extension source
  file(GLOB OGLFT_FOUNDFIFECHAN_OPENGL_HEADER include/fifechan/opengl.hpp)
  file(GLOB FIFECHAN_OPENGL_HEADERS include/fifechan/opengl/*.hpp)
  file(GLOB FIFECHAN_OPENGL_SRC src/opengl/*.cpp)

  add_library(${PROJECT_NAME}_opengl ${FIFECHAN_LIBRARY_BUILD_TYPE}
    ${FIFECHAN_OPENGL_HEADER}
    ${FIFECHAN_OPENGL_HEADERS}
    ${FIFECHAN_OPENGL_SRC}
  )

  target_link_libraries(${PROJECT_NAME}_opengl ${OPENGL_LIBRARY} ${FREETYPE_LIBRARIES} ${OGLFT_LIBRARIES} ${PROJECT_NAME})

  set_target_properties(${PROJECT_NAME}_opengl PROPERTIES
    VERSION                ${FIFECHAN_VERSION}
    SOVERSION              ${FIFECHAN_VERSION}
    CLEAN_DIRECT_OUTPUT    1                               # Allow creating static and shared libraries without conflict
    OUTPUT_NAME            ${PROJECT_NAME}_opengl          # Avoid conflicts between library and binary target names
    COMPILE_DEFINITIONS    "FIFECHAN_EXTENSION_BUILD"
    CXX_STANDARD           17
    CXX_STANDARD_REQUIRED  ON
  )

  install(TARGETS ${PROJECT_NAME}_opengl DESTINATION lib${LIB_SUFFIX} PERMISSIONS
    OWNER_READ OWNER_WRITE OWNER_EXECUTE
    GROUP_READ GROUP_EXECUTE
    WORLD_READ WORLD_EXECUTE
  )

  install(FILES ${FIFECHAN_OPENGL_HEADER}   DESTINATION include/fifechan/)
  install(FILES ${FIFECHAN_OPENGL_HEADERS}  DESTINATION include/fifechan/opengl/)

endif()

#------------------------------------------------------------------------------
# The Fifechan SDL extension library
#------------------------------------------------------------------------------

find_package(SDL2)
find_package(SDL2_image)
find_package(SDL2_ttf)

if(APPLE AND ENABLE_SDL)
  find_library(SDL2_LIBRARY SDL2)
  find_library(SDL2_IMAGE_LIBRARY SDL2_image)
  mark_as_advanced(SDL2_LIBRARY SDL2_IMAGE_LIBRARY)
  set(EXTRA_LIBS ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARY})
endif()

if(ENABLE_SDL AND SDL2_FOUND AND SDL2_IMAGE_FOUND)

  include_directories(${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIR})

  # The Fifechan SDL extension source
  file(GLOB FIFECHAN_SDL_HEADER include/fifechan/sdl.hpp)
  file(GLOB FIFECHAN_SDL_HEADERS include/fifechan/sdl/*.hpp)
  file(GLOB FIFECHAN_SDL_SRC src/sdl/*.cpp)

  include_directories(${SDL_TTF_INCLUDE_DIRS})

  add_library(${PROJECT_NAME}_sdl ${FIFECHAN_LIBRARY_BUILD_TYPE}
    ${FIFECHAN_SDL_HEADER}
    ${FIFECHAN_SDL_HEADERS}
    ${FIFECHAN_SDL_SRC}
  )

  if(WIN32)
    get_filename_component(SDL2_LIBRARY_DIR ${SDL2_LIBRARY} DIRECTORY)

    if(MINGW)
      target_link_libraries(${PROJECT_NAME}_sdl ${MINGW32_LIBRARY} ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARY} ${SDL_TTF_LIBRARY} ${PROJECT_NAME})
    else()
      target_link_libraries(${PROJECT_NAME}_sdl ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARY} ${SDL_TTF_LIBRARY} ${PROJECT_NAME})
    endif()

  else()
    target_link_libraries(${PROJECT_NAME}_sdl ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARY} ${PROJECT_NAME})
  endif()

  set_target_properties(${PROJECT_NAME}_sdl PROPERTIES
    VERSION                ${FIFECHAN_VERSION}
    SOVERSION              ${FIFECHAN_VERSION}
    CLEAN_DIRECT_OUTPUT    1                               # Allow creating static and shared libraries without conflict
    OUTPUT_NAME            ${PROJECT_NAME}_sdl             # Avoid conflicts between library and binary target names
    COMPILE_DEFINITIONS    "FIFECHAN_EXTENSION_BUILD"
    CXX_STANDARD           17
    CXX_STANDARD_REQUIRED  ON
  )

  install(TARGETS ${PROJECT_NAME}_sdl DESTINATION lib${LIB_SUFFIX} PERMISSIONS
    OWNER_READ OWNER_WRITE OWNER_EXECUTE
    GROUP_READ GROUP_EXECUTE
    WORLD_READ WORLD_EXECUTE
  )

  install(FILES ${FIFECHAN_SDL_HEADER}   DESTINATION include/fifechan/)
  install(FILES ${FIFECHAN_SDL_HEADERS}  DESTINATION include/fifechan/sdl/)

endif()
