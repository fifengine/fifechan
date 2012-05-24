CMAKE_MINIMUM_REQUIRED(VERSION 2.4.6)

# Path to local modules
SET(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake/Modules)

PROJECT(fifechan)

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
ADD_DEFINITIONS("-DFIFECHAN_BUILD -DFIFECHAN_EXTENSION_BUILD")

# The Guichan core library 
FILE(GLOB FIFECHAN_HEADER include/fifechan.hpp)
FILE(GLOB FIFECHAN_HEADERS
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
  include/fifechan/rectangle.hpp		
  include/fifechan/selectionevent.hpp	
  include/fifechan/selectionlistener.hpp	
  include/fifechan/text.hpp
  include/fifechan/visibilityeventhandler.hpp
  include/fifechan/widget.hpp		
  include/fifechan/widgetlistener.hpp
)
FILE(GLOB FIFECHAN_WIDGET_HEADERS include/fifechan/widgets/*.hpp)
FILE(GLOB FIFECHAN_CONTRIB_WIDGET_HEADERS include/fifechan/contrib/widgets/*.hpp)
FILE(GLOB FIFECHAN_SRC src/*.cpp)
FILE(GLOB FIFECHAN_WIDGET_SRC src/widgets/*.cpp)

# Grouping of the source for nicer display in IDEs such as Visual Studio
SOURCE_GROUP(src FILES ${FIFECHAN_HEADER})
SOURCE_GROUP(src/fifechan FILES ${FIFECHAN_HEADERS} ${FIFECHAN_SRC})
SOURCE_GROUP(src/fifechan/widgets FILES ${FIFECHAN_WIDGET_HEADERS} ${FIFECHAN_WIDGET_SRC})
SOURCE_GROUP(src/fifechan/contrib/widgets FILES ${FIFECHAN_CONTRIB_WIDGET_HEADERS})

OPTION(BUILD_FIFECHAN_SHARED "Build the Guichan core library as a shared library." ON)
IF(BUILD_FIFECHAN_SHARED)
  SET(FIFECHAN_LIBRARY_TYPE SHARED)
ELSE(BUILD_FIFECHAN_SHARED)
  SET(FIFECHAN_LIBRARY_TYPE STATIC)
ENDIF(BUILD_FIFECHAN_SHARED)
ADD_LIBRARY(${PROJECT_NAME} ${FIFECHAN_LIBRARY_TYPE} 
	${FIFECHAN_HEADER} 
	${FIFECHAN_HEADERS} 
	${FIFECHAN_WIDGET_HEADERS} 
	${FIFECHAN_CONTRIB_WIDGET_HEADERS}
	${FIFECHAN_SRC} 
	${FIFECHAN_WIDGET_SRC})
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
INSTALL(FILES ${FIFECHAN_HEADER} DESTINATION include/)
INSTALL(FILES ${FIFECHAN_HEADERS} DESTINATION include/fifechan/)
INSTALL(FILES ${FIFECHAN_WIDGET_HEADERS} DESTINATION include/fifechan/widgets/)
INSTALL(FILES ${FIFECHAN_CONTRIB_WIDGET_HEADERS} DESTINATION include/fifechan/contrib/widgets/)

# The Guichan Allegro extension library
OPTION(ENABLE_ALLEGRO "Enable the Guichan Allegro extension" ON)
OPTION(BUILD_FIFECHAN_ALLEGRO_SHARED "Build the Guichan Allegro extension library as a shared library." ON)
FIND_PACKAGE(Allegro)
IF(ENABLE_ALLEGRO AND ALLEGRO_FOUND)  

  INCLUDE_DIRECTORIES(${ALLEGRO_INCLUDE_DIR})

  # The Guichan Allegro extension source
  FILE(GLOB FIFECHAN_ALLEGRO_HEADER include/fifechan/allegro.hpp)
  FILE(GLOB FIFECHAN_ALLEGRO_HEADERS include/fifechan/allegro/*.hpp)
  FILE(GLOB FIFECHAN_ALLEGRO_CONTRIB_HEADERS include/fifechan/contrib/allegro/*.hpp)
  FILE(GLOB FIFECHAN_ALLEGRO_SRC src/allegro/*.cpp)

  # Grouping of the source for nicer display in IDEs such as Visual Studio
  SOURCE_GROUP(src/fifechan FILES ${FIFECHAN_ALLEGRO_HEADER})
  SOURCE_GROUP(src/fifechan/allegro FILES ${FIFECHAN_ALLEGRO_HEADERS} ${FIFECHAN_ALLEGRO_SRC})
  SOURCE_GROUP(src/fifechan/contrib/allegro FILES ${FIFECHAN_ALLEGRO_CONTRIB_HEADERS})

  IF(BUILD_FIFECHAN_ALLEGRO_SHARED)
    SET(FIFECHAN_ALLEGRO_LIBRARY_TYPE SHARED)
   ELSE(BUILD_FIFECHAN_ALLEGRO_SHARED)
     SET(FIFECHAN_ALLEGRO_LIBRARY_TYPE STATIC)
   ENDIF(BUILD_FIFECHAN_ALLEGRO_SHARED)
   ADD_LIBRARY(${PROJECT_NAME}_allegro ${FIFECHAN_ALLEGRO_LIBRARY_TYPE} 
	${FIFECHAN_ALLEGRO_HEADER}
	${FIFECHAN_ALLEGRO_HEADERS}
	${FIFECHAN_ALLEGRO_CONTRIB_HEADERS}
	${FIFECHAN_ALLEGRO_SRC})
   IF(WIN32)
     TARGET_LINK_LIBRARIES(${PROJECT_NAME}_allegro ${ALLEG_LIBRARY} ${PROJECT_NAME})
   ENDIF(WIN32)
   ADD_CUSTOM_TARGET(allegrolib DEPENDS ${PROJECT_NAME}_allegro) # Create symlink
   SET_TARGET_PROPERTIES(${PROJECT_NAME}_allegro PROPERTIES
     VERSION ${${PROJECT_NAME}_VERSION}
     SOVERSION ${${PROJECT_NAME}_SOVERSION}
     CLEAN_DIRECT_OUTPUT 1 # Allow creating static and shared libraries without conflict
     OUTPUT_NAME ${PROJECT_NAME}_allegro) # Avoid conflicts between library and binary target names 
   INSTALL(TARGETS ${PROJECT_NAME}_allegro DESTINATION lib PERMISSIONS
     OWNER_READ OWNER_WRITE OWNER_EXECUTE
     GROUP_READ GROUP_EXECUTE
     WORLD_READ WORLD_EXECUTE)
   INSTALL(FILES ${FIFECHAN_ALLEGRO_HEADER} DESTINATION include/fifechan/)
   INSTALL(FILES ${FIFECHAN_ALLEGRO_HEADERS} DESTINATION include/fifechan/allegro/)
   INSTALL(FILES ${FIFECHAN_ALLEGRO_CONTRIB_HEADERS} DESTINATION include/fifechan/contrib/allegro/)   
ENDIF(ENABLE_ALLEGRO AND ALLEGRO_FOUND)

# The Guichan Irrlicht extension library
OPTION(ENABLE_IRRLICHT "Enable the Guichan Irrlicht extension" ON)
OPTION(BUILD_FIFECHAN_IRRLICHT_SHARED "Build the Guichan Irrlicht extension library as a shared library." ON)
FIND_PACKAGE(Irrlicht)
IF(ENABLE_IRRLICHT AND IRRLICHT_FOUND)

  INCLUDE_DIRECTORIES(${IRRLICHT_INCLUDE_DIR})

  # The Guichan Irrlicht extension source
  FILE(GLOB FIFECHAN_IRRLICHT_HEADER include/fifechan/irrlicht.hpp)
  FILE(GLOB FIFECHAN_IRRLICHT_HEADERS include/fifechan/irrlicht/*.hpp)
  FILE(GLOB FIFECHAN_IRRLICHT_SRC src/irrlicht/*.cpp)

  # Grouping of the source for nicer display in IDEs such as Visual Studio
  SOURCE_GROUP(src/fifechan FILES ${FIFECHAN_IRRLICHT_HEADER})
  SOURCE_GROUP(src/fifechan/irrlicht FILES ${FIFECHAN_IRRLICHT_HEADERS} ${FIFECHAN_IRRLICHT_SRC})

  IF(BUILD_FIFECHAN_IRRLICHT_SHARED)
    SET(FIFECHAN_IRRLICHT_LIBRARY_TYPE SHARED)
   ELSE(BUILD_FIFECHAN_IRRLICHT_SHARED)
     SET(FIFECHAN_IRRLICHT_LIBRARY_TYPE STATIC)
   ENDIF(BUILD_FIFECHAN_IRRLICHT_SHARED)
   ADD_LIBRARY(${PROJECT_NAME}_irrlicht ${FIFECHAN_IRRLICHT_LIBRARY_TYPE}
        ${FIFECHAN_IRRLICHT_HEADER}
        ${FIFECHAN_IRRLICHT_HEADERS}
        ${FIFECHAN_IRRLICHT_SRC})
   IF(WIN32)
     TARGET_LINK_LIBRARIES(${PROJECT_NAME}_irrlicht ${IRRLICHT_LIBRARY} ${PROJECT_NAME})
   ENDIF(WIN32)
   ADD_CUSTOM_TARGET(irrlichtlib DEPENDS ${PROJECT_NAME}_irrlicht) # Create symlink
   SET_TARGET_PROPERTIES(${PROJECT_NAME}_irrlicht PROPERTIES
     VERSION ${${PROJECT_NAME}_VERSION}
     SOVERSION ${${PROJECT_NAME}_SOVERSION}
     CLEAN_DIRECT_OUTPUT 1 # Allow creating static and shared libraries without conflict
     OUTPUT_NAME ${PROJECT_NAME}_irrlicht) # Avoid conflicts between library and binary target names
   INSTALL(TARGETS ${PROJECT_NAME}_irrlicht DESTINATION lib PERMISSIONS
     OWNER_READ OWNER_WRITE OWNER_EXECUTE
     GROUP_READ GROUP_EXECUTE
     WORLD_READ WORLD_EXECUTE)
   INSTALL(FILES ${FIFECHAN_IRRLICHT_HEADER} DESTINATION include/fifechan/)
   INSTALL(FILES ${FIFECHAN_IRRLICHT_HEADERS} DESTINATION include/fifechan/irrlicht/)
ENDIF(ENABLE_IRRLICHT AND IRRLICHT_FOUND)

# The Guichan OpenGL extension library
OPTION(ENABLE_OPENGL "Enable the Guichan OpenGL extension" ON)
OPTION(BUILD_FIFECHAN_OPENGL_SHARED "Build the Guichan OpenGL extension library as a shared library." ON)
FIND_PACKAGE(OpenGL)

IF(APPLE AND ENABLE_OPENGL)
  FIND_LIBRARY(OPENGL_LIBRARY OpenGL)
  MARK_AS_ADVANCED(OPENGL_LIBRARY)
  SET(EXTRA_LIBS ${OPENGL_LIBRARY})
ENDIF(APPLE AND ENABLE_OPENGL)

IF(ENABLE_OPENGL AND OPENGL_FOUND)  

  INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_DIR})

  # The Guican OpenGL extension source
  FILE(GLOB FIFECHAN_OPENGL_HEADER include/fifechan/opengl.hpp)
  FILE(GLOB FIFECHAN_OPENGL_HEADERS include/fifechan/opengl/*.hpp)
  FILE(GLOB FIFECHAN_OPENGL_CONTRIB_HEADERS include/fifechan/contrib/opengl/*.hpp)
  FILE(GLOB FIFECHAN_OPENGL_SRC src/opengl/*.cpp)

  # Grouping of the source for nicer display in IDEs such as Visual Studio
  SOURCE_GROUP(src/fifechan FILES ${FIFECHAN_OPENGL_HEADER})
  SOURCE_GROUP(src/fifechan/opengl FILES ${FIFECHAN_OPENGL_HEADERS} ${FIFECHAN_OPENGL_SRC})
  SOURCE_GROUP(src/fifechan/contrib/opengl FILES ${FIFECHAN_OPENGL_CONTRIB_HEADERS})

  IF(BUILD_FIFECHAN_OPENGL_SHARED)
    SET(FIFECHAN_OPENGL_LIBRARY_TYPE SHARED)
  ELSE(BUILD_FIFECHAN_OPENGL_SHARED)
    SET(FIFECHAN_OPENGL_LIBRARY_TYPE STATIC)
  ENDIF(BUILD_FIFECHAN_OPENGL_SHARED)
  ADD_LIBRARY(${PROJECT_NAME}_opengl ${FIFECHAN_OPENGL_LIBRARY_TYPE} 
	${FIFECHAN_OPENGL_HEADER}
	${FIFECHAN_OPENGL_HEADERS}
	${FIFECHAN_OPENGL_CONTRIB_HEADERS}
	${FIFECHAN_OPENGL_SRC})
  IF(WIN32)
    TARGET_LINK_LIBRARIES(${PROJECT_NAME}_opengl ${OPENGL_LIBRARY} ${PROJECT_NAME})
  ENDIF(WIN32)
  IF(APPLE)
    TARGET_LINK_LIBRARIES(${PROJECT_NAME}_opengl ${OPENGL_LIBRARY} ${PROJECT_NAME})
  ENDIF(APPLE)

  ADD_CUSTOM_TARGET(opengllib DEPENDS ${PROJECT_NAME}_opengl) # Create symlink
  SET_TARGET_PROPERTIES(${PROJECT_NAME}_opengl PROPERTIES
    VERSION ${${PROJECT_NAME}_VERSION}
    SOVERSION ${${PROJECT_NAME}_SOVERSION}
    CLEAN_DIRECT_OUTPUT 1 # Allow creating static and shared libraries without conflict
    OUTPUT_NAME ${PROJECT_NAME}_opengl) # Avoid conflicts between library and binary target names 
  INSTALL(TARGETS ${PROJECT_NAME}_opengl DESTINATION lib PERMISSIONS
    OWNER_READ OWNER_WRITE OWNER_EXECUTE
    GROUP_READ GROUP_EXECUTE
    WORLD_READ WORLD_EXECUTE)
  INSTALL(FILES ${FIFECHAN_OPENGL_HEADER} DESTINATION include/fifechan/)
  INSTALL(FILES ${FIFECHAN_OPENGL_HEADERS} DESTINATION include/fifechan/opengl/)
  INSTALL(FILES ${FIFECHAN_OPENGL_CONTRIB_HEADERS} DESTINATION include/fifechan/contrib/opengl/)   
ENDIF(ENABLE_OPENGL AND OPENGL_FOUND)

# The Guichan SDL extension library
OPTION(ENABLE_SDL "Enable the Guichan SDL extension" ON)
OPTION(BUILD_FIFECHAN_SDL_SHARED "Build the Guichan SDL extension library as a shared library." ON)
FIND_PACKAGE(SDL)
FIND_PACKAGE(SDL_image)

IF(APPLE AND ENABLE_SDL)
  FIND_LIBRARY(SDL_LIBRARY SDL)
  FIND_LIBRARY(SDLIMAGE_LIBRARY SDL_image)
  MARK_AS_ADVANCED(SDL_LIBRARY SDLIMAGE_LIBRARY)
  SET(EXTRA_LIBS ${SDL_LIBRARY} ${SDLIMAGE_LIBRARY})
ENDIF(APPLE AND ENABLE_SDL)
IF(ENABLE_SDL AND SDL_FOUND AND SDLIMAGE_FOUND)  
 
  INCLUDE_DIRECTORIES(${SDL_INCLUDE_DIR})
 
  # The Guichan SDL extension source
  FILE(GLOB FIFECHAN_SDL_HEADER include/fifechan/sdl.hpp)
  FILE(GLOB FIFECHAN_SDL_HEADERS include/fifechan/sdl/*.hpp)
  FILE(GLOB FIFECHAN_SDL_CONTRIB_HEADERS include/fifechan/contrib/sdl/*.hpp)
  FILE(GLOB FIFECHAN_SDL_SRC src/sdl/*.cpp)
  
  # Grouping of the source for nicer display in IDEs such as Visual Studio
  SOURCE_GROUP(src/fifechan FILES ${FIFECHAN_SDL_HEADER})
  SOURCE_GROUP(src/fifechan/sdl FILES ${FIFECHAN_SDL_HEADERS} ${FIFECHAN_SDL_SRC})
  SOURCE_GROUP(src/fifechan/contrib/sdl FILES ${FIFECHAN_SDL_CONTRIB_HEADERS})
 
  IF(BUILD_FIFECHAN_SDL_SHARED)
    SET(FIFECHAN_SDL_LIBRARY_TYPE SHARED)
  ELSE(BUILD_FIFECHAN_SDL_SHARED)
    SET(FIFECHAN_SDL_LIBRARY_TYPE STATIC)
  ENDIF(BUILD_FIFECHAN_SDL_SHARED)
  ADD_LIBRARY(${PROJECT_NAME}_sdl ${FIFECHAN_SDL_LIBRARY_TYPE} 
	${FIFECHAN_SDL_HEADER}
	${FIFECHAN_SDL_HEADERS}
	${FIFECHAN_SDL_CONTRIB_HEADERS}
	${FIFECHAN_SDL_SRC})
  IF(WIN32)
    IF(MINGW)
      TARGET_LINK_LIBRARIES(${PROJECT_NAME}_sdl ${MINGW32_LIBRARY} ${SDL_LIBRARY} ${SDLIMAGE_LIBRARY} SDLmain ${PROJECT_NAME})
    ELSE(MINGW)
      TARGET_LINK_LIBRARIES(${PROJECT_NAME}_sdl ${SDL_LIBRARY} ${SDLIMAGE_LIBRARY} SDLmain ${PROJECT_NAME})
    ENDIF(MINGW)
  ENDIF(WIN32)
  IF(APPLE)
    TARGET_LINK_LIBRARIES(${PROJECT_NAME}_sdl ${SDL_LIBRARY} ${SDLIMAGE_LIBRARY} SDLmain ${PROJECT_NAME})
  ENDIF(APPLE)
  ADD_CUSTOM_TARGET(sdllib DEPENDS ${PROJECT_NAME}_sdl) # Create symlink
  SET_TARGET_PROPERTIES(${PROJECT_NAME}_sdl PROPERTIES
    VERSION ${${PROJECT_NAME}_VERSION}
    SOVERSION ${${PROJECT_NAME}_SOVERSION}
    CLEAN_DIRECT_OUTPUT 1 # Allow creating static and shared libraries without conflict
    OUTPUT_NAME ${PROJECT_NAME}_sdl) # Avoid conflicts between library and binary target names 
  INSTALL(TARGETS ${PROJECT_NAME}_sdl DESTINATION lib PERMISSIONS
    OWNER_READ OWNER_WRITE OWNER_EXECUTE
    GROUP_READ GROUP_EXECUTE
    WORLD_READ WORLD_EXECUTE)
  INSTALL(FILES ${FIFECHAN_SDL_HEADER} DESTINATION include/fifechan/)
  INSTALL(FILES ${FIFECHAN_SDL_HEADERS} DESTINATION include/fifechan/sdl/)
  INSTALL(FILES ${FIFECHAN_SDL_CONTRIB_HEADERS} DESTINATION include/fifechan/contrib/sdl/)   
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
