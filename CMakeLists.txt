CMAKE_MINIMUM_REQUIRED(VERSION 2.4.6)

# Path to local modules
SET(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake/Modules)

PROJECT(guichan)

SET(MAJOR_VERSION 0)
SET(MINOR_VERSION 9)
SET(PATCH_LEVEL 0)
SET(${PROJECT_NAME}_SOVERSION ${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_LEVEL})
SET(${PROJECT_NAME}_VERSION ${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_LEVEL})

# set default cmake build type to RelWithDebInfo (None Debug Release RelWithDebInfo MinSizeRel)
#IF( NOT CMAKE_BUILD_TYPE )
#  SET( CMAKE_BUILD_TYPE "RelWithDebInfo" )
#ENDIF()

INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/include")
ADD_DEFINITIONS("-DGUICHAN_BUILD -DGUICHAN_EXTENSION_BUILD")

# The Guichan core library 
FILE(GLOB GUICHAN_HEADER include/guichan.hpp)
FILE(GLOB GUICHAN_HEADERS
  include/guichan/actionevent.hpp		
  include/guichan/actionlistener.hpp		
  include/guichan/cliprectangle.hpp	
  include/guichan/color.hpp		
  include/guichan/containerevent.hpp		
  include/guichan/containerlistener.hpp		
  include/guichan/deathlistener.hpp	
  include/guichan/defaultfont.hpp		
  include/guichan/event.hpp		
  include/guichan/exception.hpp		
  include/guichan/focushandler.hpp	
  include/guichan/focuslistener.hpp	
  include/guichan/font.hpp		
  include/guichan/genericinput.hpp	
  include/guichan/glut.hpp		
  include/guichan/graphics.hpp		
  include/guichan/gui.hpp			
  include/guichan/image.hpp		
  include/guichan/imagefont.hpp		
  include/guichan/imageloader.hpp		
  include/guichan/input.hpp		
  include/guichan/inputevent.hpp		
  include/guichan/key.hpp			
  include/guichan/keyevent.hpp		
  include/guichan/keyinput.hpp		
  include/guichan/keylistener.hpp		
  include/guichan/listmodel.hpp		
  include/guichan/mouseevent.hpp		
  include/guichan/mouseinput.hpp		
  include/guichan/mouselistener.hpp	
  include/guichan/platform.hpp		
  include/guichan/rectangle.hpp		
  include/guichan/selectionevent.hpp	
  include/guichan/selectionlistener.hpp	
  include/guichan/text.hpp
  include/guichan/widget.hpp		
  include/guichan/widgetlistener.hpp
)
FILE(GLOB GUICHAN_WIDGET_HEADERS include/guichan/widgets/*.hpp)
FILE(GLOB GUICHAN_CONTRIB_WIDGET_HEADERS include/guichan/contrib/widgets/*.hpp)
FILE(GLOB GUICHAN_SRC src/*.cpp)
FILE(GLOB GUICHAN_WIDGET_SRC src/widgets/*.cpp)

# Grouping of the source for nicer display in IDEs such as Visual Studio
SOURCE_GROUP(src FILES ${GUICHAN_HEADER})
SOURCE_GROUP(src/guichan FILES ${GUICHAN_HEADERS} ${GUICHAN_SRC})
SOURCE_GROUP(src/guichan/widgets FILES ${GUICHAN_WIDGET_HEADERS} ${GUICHAN_WIDGET_SRC})
SOURCE_GROUP(src/guichan/contrib/widgets FILES ${GUICHAN_CONTRIB_WIDGET_HEADERS})

OPTION(BUILD_GUICHAN_SHARED "Build the Guichan core library as a shared library." ON)
IF(BUILD_GUICHAN_SHARED)
  SET(GUICHAN_LIBRARY_TYPE SHARED)
ELSE(BUILD_GUICHAN_SHARED)
  SET(GUICHAN_LIBRARY_TYPE STATIC)
ENDIF(BUILD_GUICHAN_SHARED)
ADD_LIBRARY(${PROJECT_NAME} ${GUICHAN_LIBRARY_TYPE} 
	${GUICHAN_HEADER} 
	${GUICHAN_HEADERS} 
	${GUICHAN_WIDGET_HEADERS} 
	${GUICHAN_CONTRIB_WIDGET_HEADERS}
	${GUICHAN_SRC} 
	${GUICHAN_WIDGET_SRC})
ADD_CUSTOM_TARGET(lib DEPENDS ${PROJECT_NAME}) # Create symlink
SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES
   VERSION ${${PROJECT_NAME}_VERSION}
   SOVERSION ${${PROJECT_NAME}_SOVERSION}
   CLEAN_DIRECT_OUTPUT 1 # Allow creating static and shared libraries without conflict
   OUTPUT_NAME ${PROJECT_NAME}) # Avoid conflicts between library and binary target names 
INSTALL(TARGETS ${PROJECT_NAME} DESTINATION lib PERMISSIONS
  OWNER_READ OWNER_WRITE OWNER_EXECUTE
  GROUP_READ GROUP_EXECUTE
  WORLD_READ WORLD_EXECUTE)
INSTALL(FILES ${GUICHAN_HEADER} DESTINATION include/)
INSTALL(FILES ${GUICHAN_HEADERS} DESTINATION include/guichan/)
INSTALL(FILES ${GUICHAN_WIDGET_HEADERS} DESTINATION include/guichan/widgets/)
INSTALL(FILES ${GUICHAN_CONTRIB_WIDGET_HEADERS} DESTINATION include/guichan/contrib/widgets/)

# The Guichan Allegro extension library
OPTION(ENABLE_ALLEGRO "Enable the Guichan Allegro extension" ON)
OPTION(BUILD_GUICHAN_ALLEGRO_SHARED "Build the Guichan Allegro extension library as a shared library." ON)
FIND_PACKAGE(Allegro)
IF(ENABLE_ALLEGRO AND ALLEGRO_FOUND)  

  INCLUDE_DIRECTORIES(${ALLEGRO_INCLUDE_DIR})

  # The Guichan Allegro extension source
  FILE(GLOB GUICHAN_ALLEGRO_HEADER include/guichan/allegro.hpp)
  FILE(GLOB GUICHAN_ALLEGRO_HEADERS include/guichan/allegro/*.hpp)
  FILE(GLOB GUICHAN_ALLEGRO_CONTRIB_HEADERS include/guichan/contrib/allegro/*.hpp)
  FILE(GLOB GUICHAN_ALLEGRO_SRC src/allegro/*.cpp)

  # Grouping of the source for nicer display in IDEs such as Visual Studio
  SOURCE_GROUP(src/guichan FILES ${GUICHAN_ALLEGRO_HEADER})
  SOURCE_GROUP(src/guichan/allegro FILES ${GUICHAN_ALLEGRO_HEADERS} ${GUICHAN_ALLEGRO_SRC})
  SOURCE_GROUP(src/guichan/contrib/allegro FILES ${GUICHAN_ALLEGRO_CONTRIB_HEADERS})

  IF(BUILD_GUICHAN_ALLEGRO_SHARED)
    SET(GUICHAN_ALLEGRO_LIBRARY_TYPE SHARED)
   ELSE(BUILD_GUICHAN_ALLEGRO_SHARED)
     SET(GUICHAN_ALLEGRO_LIBRARY_TYPE STATIC)
   ENDIF(BUILD_GUICHAN_ALLEGRO_SHARED)
   ADD_LIBRARY(${PROJECT_NAME}_allegro ${GUICHAN_ALLEGRO_LIBRARY_TYPE} 
	${GUICHAN_ALLEGRO_HEADER}
	${GUICHAN_ALLEGRO_HEADERS}
	${GUICHAN_ALLEGRO_CONTRIB_HEADERS}
	${GUICHAN_ALLEGRO_SRC})
   IF(WIN32)
     TARGET_LINK_LIBRARIES(${PROJECT_NAME}_allegro ${ALLEG_LIBRARY} ${PROJECT_NAME})
   ENDIF(WIN32)
   ADD_CUSTOM_TARGET(lib DEPENDS ${PROJECT_NAME}_allegro) # Create symlink
   SET_TARGET_PROPERTIES(${PROJECT_NAME}_allegro PROPERTIES
     VERSION ${${PROJECT_NAME}_VERSION}
     SOVERSION ${${PROJECT_NAME}_SOVERSION}
     CLEAN_DIRECT_OUTPUT 1 # Allow creating static and shared libraries without conflict
     OUTPUT_NAME ${PROJECT_NAME}_allegro) # Avoid conflicts between library and binary target names 
   INSTALL(TARGETS ${PROJECT_NAME}_allegro DESTINATION lib PERMISSIONS
     OWNER_READ OWNER_WRITE OWNER_EXECUTE
     GROUP_READ GROUP_EXECUTE
     WORLD_READ WORLD_EXECUTE)
   INSTALL(FILES ${GUICHAN_ALLEGRO_HEADER} DESTINATION include/guichan/)
   INSTALL(FILES ${GUICHAN_ALLEGRO_HEADERS} DESTINATION include/guichan/allegro/)
   INSTALL(FILES ${GUICHAN_ALLEGRO_CONTRIB_HEADERS} DESTINATION include/guichan/contrib/allegro/)   
ENDIF(ENABLE_ALLEGRO AND ALLEGRO_FOUND)

# The Guichan Irrlicht extension library
OPTION(ENABLE_IRRLICHT "Enable the Guichan Irrlicht extension" ON)
OPTION(BUILD_GUICHAN_IRRLICHT_SHARED "Build the Guichan Irrlicht extension library as a shared library." ON)
FIND_PACKAGE(Irrlicht)
IF(ENABLE_IRRLICHT AND IRRLICHT_FOUND)

  INCLUDE_DIRECTORIES(${IRRLICHT_INCLUDE_DIR})

  # The Guichan Irrlicht extension source
  FILE(GLOB GUICHAN_IRRLICHT_HEADER include/guichan/irrlicht.hpp)
  FILE(GLOB GUICHAN_IRRLICHT_HEADERS include/guichan/irrlicht/*.hpp)
  FILE(GLOB GUICHAN_IRRLICHT_SRC src/irrlicht/*.cpp)

  # Grouping of the source for nicer display in IDEs such as Visual Studio
  SOURCE_GROUP(src/guichan FILES ${GUICHAN_IRRLICHT_HEADER})
  SOURCE_GROUP(src/guichan/irrlicht FILES ${GUICHAN_IRRLICHT_HEADERS} ${GUICHAN_IRRLICHT_SRC})

  IF(BUILD_GUICHAN_IRRLICHT_SHARED)
    SET(GUICHAN_IRRLICHT_LIBRARY_TYPE SHARED)
   ELSE(BUILD_GUICHAN_IRRLICHT_SHARED)
     SET(GUICHAN_IRRLICHT_LIBRARY_TYPE STATIC)
   ENDIF(BUILD_GUICHAN_IRRLICHT_SHARED)
   ADD_LIBRARY(${PROJECT_NAME}_irrlicht ${GUICHAN_IRRLICHT_LIBRARY_TYPE}
        ${GUICHAN_IRRLICHT_HEADER}
        ${GUICHAN_IRRLICHT_HEADERS}
        ${GUICHAN_IRRLICHT_SRC})
   IF(WIN32)
     TARGET_LINK_LIBRARIES(${PROJECT_NAME}_irrlicht ${IRRLICHT_LIBRARY} ${PROJECT_NAME})
   ENDIF(WIN32)
   ADD_CUSTOM_TARGET(lib DEPENDS ${PROJECT_NAME}_irrlicht) # Create symlink
   SET_TARGET_PROPERTIES(${PROJECT_NAME}_irrlicht PROPERTIES
     VERSION ${${PROJECT_NAME}_VERSION}
     SOVERSION ${${PROJECT_NAME}_SOVERSION}
     CLEAN_DIRECT_OUTPUT 1 # Allow creating static and shared libraries without conflict
     OUTPUT_NAME ${PROJECT_NAME}_irrlicht) # Avoid conflicts between library and binary target names
   INSTALL(TARGETS ${PROJECT_NAME}_irrlicht DESTINATION lib PERMISSIONS
     OWNER_READ OWNER_WRITE OWNER_EXECUTE
     GROUP_READ GROUP_EXECUTE
     WORLD_READ WORLD_EXECUTE)
   INSTALL(FILES ${GUICHAN_IRRLICHT_HEADER} DESTINATION include/guichan/)
   INSTALL(FILES ${GUICHAN_IRRLICHT_HEADERS} DESTINATION include/guichan/irrlicht/)
ENDIF(ENABLE_IRRLICHT AND IRRLICHT_FOUND)

# The Guichan OpenGL extension library
OPTION(ENABLE_OPENGL "Enable the Guichan OpenGL extension" ON)
OPTION(BUILD_GUICHAN_OPENGL_SHARED "Build the Guichan OpenGL extension library as a shared library." ON)
FIND_PACKAGE(OpenGL)
IF(ENABLE_OPENGL AND OPENGL_FOUND)  

  INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_DIR})

  # The Guican OpenGL extension source
  FILE(GLOB GUICHAN_OPENGL_HEADER include/guichan/opengl.hpp)
  FILE(GLOB GUICHAN_OPENGL_HEADERS include/guichan/opengl/*.hpp)
  FILE(GLOB GUICHAN_OPENGL_CONTRIB_HEADERS include/guichan/contrib/opengl/*.hpp)
  FILE(GLOB GUICHAN_OPENGL_SRC src/opengl/*.cpp)

  # Grouping of the source for nicer display in IDEs such as Visual Studio
  SOURCE_GROUP(src/guichan FILES ${GUICHAN_OPENGL_HEADER})
  SOURCE_GROUP(src/guichan/opengl FILES ${GUICHAN_OPENGL_HEADERS} ${GUICHAN_OPENGL_SRC})
  SOURCE_GROUP(src/guichan/contrib/opengl FILES ${GUICHAN_OPENGL_CONTRIB_HEADERS})

  IF(BUILD_GUICHAN_OPENGL_SHARED)
    SET(GUICHAN_OPENGL_LIBRARY_TYPE SHARED)
  ELSE(BUILD_GUICHAN_OPENGL_SHARED)
    SET(GUICHAN_OPENGL_LIBRARY_TYPE STATIC)
  ENDIF(BUILD_GUICHAN_OPENGL_SHARED)
  ADD_LIBRARY(${PROJECT_NAME}_opengl ${GUICHAN_OPENGL_LIBRARY_TYPE} 
	${GUICHAN_OPENGL_HEADER}
	${GUICHAN_OPENGL_HEADERS}
	${GUICHAN_OPENGL_CONTRIB_HEADERS}
	${GUICHAN_OPENGL_SRC})
  IF(WIN32)
    TARGET_LINK_LIBRARIES(${PROJECT_NAME}_opengl ${OPENGL_LIBRARY} ${PROJECT_NAME})
  ENDIF(WIN32)
  ADD_CUSTOM_TARGET(lib DEPENDS ${PROJECT_NAME}_opengl) # Create symlink
  SET_TARGET_PROPERTIES(${PROJECT_NAME}_opengl PROPERTIES
    VERSION ${${PROJECT_NAME}_VERSION}
    SOVERSION ${${PROJECT_NAME}_SOVERSION}
    CLEAN_DIRECT_OUTPUT 1 # Allow creating static and shared libraries without conflict
    OUTPUT_NAME ${PROJECT_NAME}_opengl) # Avoid conflicts between library and binary target names 
  INSTALL(TARGETS ${PROJECT_NAME}_opengl DESTINATION lib PERMISSIONS
    OWNER_READ OWNER_WRITE OWNER_EXECUTE
    GROUP_READ GROUP_EXECUTE
    WORLD_READ WORLD_EXECUTE)
  INSTALL(FILES ${GUICHAN_OPENGL_HEADER} DESTINATION include/guichan/)
  INSTALL(FILES ${GUICHAN_OPENGL_HEADERS} DESTINATION include/guichan/opengl/)
  INSTALL(FILES ${GUICHAN_OPENGL_CONTRIB_HEADERS} DESTINATION include/guichan/contrib/opengl/)   
ENDIF(ENABLE_OPENGL AND OPENGL_FOUND)

# The Guichan SDL extension library
OPTION(ENABLE_SDL "Enable the Guichan SDL extension" ON)
OPTION(BUILD_GUICHAN_SDL_SHARED "Build the Guichan SDL extension library as a shared library." ON)
FIND_PACKAGE(SDL)
FIND_PACKAGE(SDL_image)
IF(ENABLE_SDL AND SDL_FOUND AND SDLIMAGE_FOUND)  
 
  INCLUDE_DIRECTORIES(${SDL_INCLUDE_DIR})
 
  # The Guichan SDL extension source
  FILE(GLOB GUICHAN_SDL_HEADER include/guichan/sdl.hpp)
  FILE(GLOB GUICHAN_SDL_HEADERS include/guichan/sdl/*.hpp)
  FILE(GLOB GUICHAN_SDL_CONTRIB_HEADERS include/guichan/contrib/sdl/*.hpp)
  FILE(GLOB GUICHAN_SDL_SRC src/sdl/*.cpp)
  
  # Grouping of the source for nicer display in IDEs such as Visual Studio
  SOURCE_GROUP(src/guichan FILES ${GUICHAN_SDL_HEADER})
  SOURCE_GROUP(src/guichan/sdl FILES ${GUICHAN_SDL_HEADERS} ${GUICHAN_SDL_SRC})
  SOURCE_GROUP(src/guichan/contrib/sdl FILES ${GUICHAN_SDL_CONTRIB_HEADERS})
 
  IF(BUILD_GUICHAN_SDL_SHARED)
    SET(GUICHAN_SDL_LIBRARY_TYPE SHARED)
  ELSE(BUILD_GUICHAN_SDL_SHARED)
    SET(GUICHAN_SDL_LIBRARY_TYPE STATIC)
  ENDIF(BUILD_GUICHAN_SDL_SHARED)
  ADD_LIBRARY(${PROJECT_NAME}_sdl ${GUICHAN_SDL_LIBRARY_TYPE} 
	${GUICHAN_SDL_HEADER}
	${GUICHAN_SDL_HEADERS}
	${GUICHAN_SDL_CONTRIB_HEADERS}
	${GUICHAN_SDL_SRC})
  IF(WIN32)
    IF(MINGW)
      TARGET_LINK_LIBRARIES(${PROJECT_NAME}_sdl ${MINGW32_LIBRARY} ${SDL_LIBRARY} ${SDLIMAGE_LIBRARY} SDLmain ${PROJECT_NAME})
    ELSE(MINGW)
      TARGET_LINK_LIBRARIES(${PROJECT_NAME}_sdl ${SDL_LIBRARY} ${SDLIMAGE_LIBRARY} SDLmain ${PROJECT_NAME})
    ENDIF(MINGW)
  ENDIF(WIN32)
  ADD_CUSTOM_TARGET(lib DEPENDS ${PROJECT_NAME}_sdl) # Create symlink
  SET_TARGET_PROPERTIES(${PROJECT_NAME}_sdl PROPERTIES
    VERSION ${${PROJECT_NAME}_VERSION}
    SOVERSION ${${PROJECT_NAME}_SOVERSION}
    CLEAN_DIRECT_OUTPUT 1 # Allow creating static and shared libraries without conflict
    OUTPUT_NAME ${PROJECT_NAME}_sdl) # Avoid conflicts between library and binary target names 
  INSTALL(TARGETS ${PROJECT_NAME}_sdl DESTINATION lib PERMISSIONS
    OWNER_READ OWNER_WRITE OWNER_EXECUTE
    GROUP_READ GROUP_EXECUTE
    WORLD_READ WORLD_EXECUTE)
  INSTALL(FILES ${GUICHAN_SDL_HEADER} DESTINATION include/guichan/)
  INSTALL(FILES ${GUICHAN_SDL_HEADERS} DESTINATION include/guichan/sdl/)
  INSTALL(FILES ${GUICHAN_SDL_CONTRIB_HEADERS} DESTINATION include/guichan/contrib/sdl/)   
ENDIF(ENABLE_SDL AND SDL_FOUND AND SDLIMAGE_FOUND)

# Make uninstall target
#CONFIGURE_FILE(
#  "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
#  "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
#  IMMEDIATE @ONLY)
#ADD_CUSTOM_TARGET(uninstall
#  "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")

MESSAGE(STATUS "Guichan has been built, hooray!")
# MESSAGE( STATUS "CMAKE_INSTALL_PREFIX = ${CMAKE_INSTALL_PREFIX}" )
# MESSAGE( STATUS "CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE}" )
# MESSAGE( STATUS "CMAKE_MODULE_PATH = ${CMAKE_MODULE_PATH}" )
# MESSAGE( STATUS "${PROJECT_NAME}_DEPENDS = \"${${PROJECT_NAME}_DEPENDS}\"" )
# MESSAGE( STATUS "BUILD_WITH = \"${BUILD_WITH}\"" )
# MESSAGE( STATUS "INSTALL_DOC = ${INSTALL_DOC}" )
# MESSAGE( STATUS "Change a value with: cmake -D<Variable>=<Value>" )
# MESSAGE( STATUS "-------------------------------------------------------------------------------" )
# MESSAGE( STATUS )

# # force some variables that could be defined in the command line to be written to cache
#SET(BUILD_SHARED_LIBS "${BUILD_SHARED_LIBS}" CACHE BOOL "Set to OFF to build static libraries" FORCE)
# SET( CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}" CACHE PATH
#   "Where to install ${PROJECT_NAME}" FORCE )
# SET( CMAKE_BUILD_TYPE "${CMAKE_BUILD_TYPE}" CACHE STRING
#   "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel." FORCE )
# SET( CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" CACHE PATH
#   "Path to custom CMake Modules" FORCE )
# SET( INSTALL_DOC "${INSTALL_DOC}" CACHE BOOL
#   "Set to OFF to skip build/install Documentation" FORCE )
# # export build settings
# INCLUDE( CMakeExportBuildSettings )
# CMAKE_EXPORT_BUILD_SETTINGS( "${PROJECT_NAME}BuildSettings.cmake" )
# # export library dependencies (keep this as the last line in the file)
# EXPORT_LIBRARY_DEPENDENCIES( "${PROJECT_NAME}LibDeps.cmake" )
