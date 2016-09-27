#------------------------------------------------------------------------------
#                                 FifeChan / FifeGUI                                         
#------------------------------------------------------------------------------

CMAKE_MINIMUM_REQUIRED(VERSION 2.6.0)

PROJECT(fifechan)

#------------------------------------------------------------------------------
#                                 Version                                          
#------------------------------------------------------------------------------

SET(MAJOR_VERSION 0)
SET(MINOR_VERSION 1)
SET(PATCH_VERSION 2)
SET(FIFECHAN_VERSION ${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_VERSION})

SET(LIB_SUFFIX "" CACHE STRING "Suffix to append to the system lib folder")

#------------------------------------------------------------------------------
#                                 Options                                          
#------------------------------------------------------------------------------

OPTION(BUILD_FIFECHAN_SHARED            "Build the Fifechan core library as a shared library"                ON)

OPTION(ENABLE_ALLEGRO                   "Enable the Fifechan Allegro extension"                              ON)
OPTION(BUILD_FIFECHAN_ALLEGRO_SHARED    "Build the Fifechan Allegro extension library as a shared library"   ON)

OPTION(ENABLE_IRRLICHT                  "Enable the Fifechan Irrlicht extension"                             ON)
OPTION(BUILD_FIFECHAN_IRRLICHT_SHARED   "Build the Fifechan Irrlicht extension library as a shared library"  ON)

OPTION(ENABLE_OPENGL                    "Enable the Fifechan OpenGL extension"                               ON)
OPTION(ENABLE_OPENGL_CONTRIB            "Enable the Fifechan OpenGL contrib extension (freetype, oglft)"     OFF)
OPTION(BUILD_FIFECHAN_OPENGL_SHARED     "Build the Fifechan OpenGL extension library as a shared library"    ON)

OPTION(ENABLE_SDL                       "Enable the Fifechan SDL extension"                                  ON)
OPTION(ENABLE_SDL_CONTRIB               "Enable the Fifechan SDL contrib extension (SDL2_ttf)"               OFF)
OPTION(BUILD_FIFECHAN_SDL_SHARED        "Build the Fifechan SDL extension library as a shared library"       ON)

#------------------------------------------------------------------------------
#                                 Display Status                                         
#------------------------------------------------------------------------------

message("")
message("== Building FIFECHAN version ${FIFECHAN_VERSION} using build type '${CMAKE_BUILD_TYPE}'.")
message(STATUS "    Source directory is '${PROJECT_SOURCE_DIR}'.")
message(STATUS "    Build  directory is '${PROJECT_BINARY_DIR}'.")
message(STATUS "    Installation path is '${CMAKE_INSTALL_PREFIX}' (set -DCMAKE_INSTALL_PREFIX=<path> to change).")
message("")

#------------------------------------------------------------------------------
#                           Find Dependencies - Windows                                        
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
endif(WIN32)

#------------------------------------------------------------------------------
#                                 Configure                                          
#------------------------------------------------------------------------------

# Path to local modules
SET(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake/Modules)

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

#------------------------------------------------------------------------------
#                                 Source Files                                             
#------------------------------------------------------------------------------

INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/include")

# The Fifechan core library 
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
  include/fifechan/fixedsizeconstraint.hpp  
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
  include/fifechan/size.hpp
  include/fifechan/sizeconstraint.hpp
  include/fifechan/text.hpp
  include/fifechan/utf8stringeditor.hpp
  include/fifechan/visibilityeventhandler.hpp
  include/fifechan/widget.hpp		
  include/fifechan/widgetlistener.hpp
  include/fifechan/opengl.hpp
)
FILE(GLOB FIFECHAN_WIDGET_HEADERS include/fifechan/widgets/*.hpp)
FILE(GLOB FIFECHAN_CONTRIB_WIDGET_HEADERS include/fifechan/contrib/widgets/*.hpp)
FILE(GLOB FIFECHAN_SRC src/*.cpp)
FILE(GLOB FIFECHAN_WIDGET_SRC src/widgets/*.cpp)

# Grouping of the source for nicer display in IDEs such as Visual Studio
SOURCE_GROUP(src                          FILES ${FIFECHAN_HEADER})
SOURCE_GROUP(src/fifechan                 FILES ${FIFECHAN_HEADERS} ${FIFECHAN_SRC})
SOURCE_GROUP(src/fifechan/widgets         FILES ${FIFECHAN_WIDGET_HEADERS} ${FIFECHAN_WIDGET_SRC})
SOURCE_GROUP(src/fifechan/contrib/widgets FILES ${FIFECHAN_CONTRIB_WIDGET_HEADERS})

#------------------------------------------------------------------------------
#                 Fifechan core library as a shared library                                        
#------------------------------------------------------------------------------

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
	${FIFECHAN_WIDGET_SRC}
)

ADD_CUSTOM_TARGET(lib DEPENDS ${PROJECT_NAME}) # Create symlink

SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES
  VERSION                 ${FIFECHAN_VERSION}
  SOVERSION               ${FIFECHAN_VERSION}
  CLEAN_DIRECT_OUTPUT     1                             # Allow creating static and shared libraries without conflict
  OUTPUT_NAME             ${PROJECT_NAME}               # Avoid conflicts between library and binary target names 
  COMPILE_DEFINITIONS     "FIFECHAN_BUILD"
)

INSTALL(TARGETS ${PROJECT_NAME} DESTINATION lib${LIB_SUFFIX} PERMISSIONS
  OWNER_READ OWNER_WRITE OWNER_EXECUTE
  GROUP_READ GROUP_EXECUTE
  WORLD_READ WORLD_EXECUTE
)

INSTALL(FILES ${FIFECHAN_HEADER}                  DESTINATION include/)
INSTALL(FILES ${FIFECHAN_HEADERS}                 DESTINATION include/fifechan/)
INSTALL(FILES ${FIFECHAN_WIDGET_HEADERS}          DESTINATION include/fifechan/widgets/)
INSTALL(FILES ${FIFECHAN_CONTRIB_WIDGET_HEADERS}  DESTINATION include/fifechan/contrib/widgets/)

#REMOVE_DEFINITIONS("-DFIFECHAN_BUILD")
#ADD_DEFINITIONS("-DFIFECHAN_EXTENSION_BUILD")

#------------------------------------------------------------------------------
#                   The Fifechan Allegro extension library                                         
#------------------------------------------------------------------------------

FIND_PACKAGE(Allegro)

IF(ENABLE_ALLEGRO AND ALLEGRO_FOUND)  

  INCLUDE_DIRECTORIES(${ALLEGRO_INCLUDE_DIR})

  # The Fifechan Allegro extension source
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
  	${FIFECHAN_ALLEGRO_SRC}
  )

  TARGET_LINK_LIBRARIES(${PROJECT_NAME}_allegro ${ALLEG_LIBRARY} ${PROJECT_NAME})
  
  ADD_CUSTOM_TARGET(allegrolib DEPENDS ${PROJECT_NAME}_allegro) # Create symlink
  
  SET_TARGET_PROPERTIES(${PROJECT_NAME}_allegro PROPERTIES
    VERSION                   ${FIFECHAN_VERSION}
    SOVERSION                 ${FIFECHAN_VERSION}
    CLEAN_DIRECT_OUTPUT       1                               # Allow creating static and shared libraries without conflict
    OUTPUT_NAME               ${PROJECT_NAME}_allegro         # Avoid conflicts between library and binary target names 
    COMPILE_DEFINITIONS       "FIFECHAN_EXTENSION_BUILD"
  )

  INSTALL(TARGETS ${PROJECT_NAME}_allegro DESTINATION lib${LIB_SUFFIX} PERMISSIONS
    OWNER_READ OWNER_WRITE OWNER_EXECUTE
    GROUP_READ GROUP_EXECUTE
    WORLD_READ WORLD_EXECUTE
  )

  INSTALL(FILES ${FIFECHAN_ALLEGRO_HEADER}            DESTINATION include/fifechan/)
  INSTALL(FILES ${FIFECHAN_ALLEGRO_HEADERS}           DESTINATION include/fifechan/allegro/)
  INSTALL(FILES ${FIFECHAN_ALLEGRO_CONTRIB_HEADERS}   DESTINATION include/fifechan/contrib/allegro/)  

ENDIF(ENABLE_ALLEGRO AND ALLEGRO_FOUND)

#------------------------------------------------------------------------------
#                   Fifechan Irrlicht extension library                                         
#------------------------------------------------------------------------------

FIND_PACKAGE(Irrlicht)
IF(ENABLE_IRRLICHT AND IRRLICHT_FOUND)

  INCLUDE_DIRECTORIES(${IRRLICHT_INCLUDE_DIR})

  # The Fifechan Irrlicht extension source
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
    ${FIFECHAN_IRRLICHT_SRC}
  )

  TARGET_LINK_LIBRARIES(${PROJECT_NAME}_irrlicht ${IRRLICHT_LIBRARY} ${PROJECT_NAME})

  ADD_CUSTOM_TARGET(irrlichtlib DEPENDS ${PROJECT_NAME}_irrlicht) # Create symlink

  SET_TARGET_PROPERTIES(${PROJECT_NAME}_irrlicht PROPERTIES
    VERSION                  ${FIFECHAN_VERSION}
    SOVERSION                ${FIFECHAN_VERSION}
    CLEAN_DIRECT_OUTPUT      1                               # Allow creating static and shared libraries without conflict
    OUTPUT_NAME              ${PROJECT_NAME}_irrlicht        # Avoid conflicts between library and binary target names
    COMPILE_DEFINITIONS      "FIFECHAN_EXTENSION_BUILD"
  )

  INSTALL(TARGETS ${PROJECT_NAME}_irrlicht DESTINATION lib${LIB_SUFFIX} PERMISSIONS
    OWNER_READ OWNER_WRITE OWNER_EXECUTE
    GROUP_READ GROUP_EXECUTE
    WORLD_READ WORLD_EXECUTE
  )

  INSTALL(FILES ${FIFECHAN_IRRLICHT_HEADER}    DESTINATION include/fifechan/)
  INSTALL(FILES ${FIFECHAN_IRRLICHT_HEADERS}   DESTINATION include/fifechan/irrlicht/)

ENDIF(ENABLE_IRRLICHT AND IRRLICHT_FOUND)

#------------------------------------------------------------------------------
#                   The Fifechan OpenGL extension library                                         
#------------------------------------------------------------------------------

FIND_PACKAGE(OpenGL)

IF(APPLE AND ENABLE_OPENGL)
  FIND_LIBRARY(OPENGL_LIBRARY OpenGL)
  MARK_AS_ADVANCED(OPENGL_LIBRARY)
  SET(EXTRA_LIBS ${OPENGL_LIBRARY})
ENDIF(APPLE AND ENABLE_OPENGL)

IF(ENABLE_OPENGL_CONTRIB)
  FIND_PACKAGE(Freetype)
  FIND_PACKAGE(OGLFT)
ENDIF(ENABLE_OPENGL_CONTRIB)

IF(ENABLE_OPENGL AND OPENGL_FOUND)

  INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_DIR})
  IF(ENABLE_OPENGL_CONTRIB AND OGLFT_FOUND AND FREETYPE_FOUND)
    INCLUDE_DIRECTORIES(${FREETYPE_INCLUDE_DIRS})
    INCLUDE_DIRECTORIES(${OGLFT_INCLUDE_DIRS})
  ENDIF(ENABLE_OPENGL_CONTRIB AND OGLFT_FOUND AND FREETYPE_FOUND)

  # The OpenGL extension source
  FILE(GLOB OGLFT_FOUNDFIFECHAN_OPENGL_HEADER include/fifechan/opengl.hpp)
  FILE(GLOB FIFECHAN_OPENGL_HEADERS include/fifechan/opengl/*.hpp)
  FILE(GLOB FIFECHAN_OPENGL_CONTRIB_HEADERS include/fifechan/contrib/opengl/*.hpp)
  FILE(GLOB FIFECHAN_OPENGL_SRC src/opengl/*.cpp)
  IF(ENABLE_OPENGL_CONTRIB)
    FILE(GLOB FIFECHAN_OPENGL_CONTRIB_SRC src/contrib/opengl/*.cpp)
  ENDIF(ENABLE_OPENGL_CONTRIB)

  # Grouping of the source for nicer display in IDEs such as Visual Studio
  SOURCE_GROUP(src/fifechan                FILES ${FIFECHAN_OPENGL_HEADER})
  SOURCE_GROUP(src/fifechan/opengl         FILES ${FIFECHAN_OPENGL_HEADERS} ${FIFECHAN_OPENGL_SRC})
  SOURCE_GROUP(src/fifechan/contrib/opengl FILES ${FIFECHAN_OPENGL_CONTRIB_HEADERS} ${FIFECHAN_OPENGL_CONTRIB_SRC})

  IF(BUILD_FIFECHAN_OPENGL_SHARED)
    SET(FIFECHAN_OPENGL_LIBRARY_TYPE SHARED)
  ELSE(BUILD_FIFECHAN_OPENGL_SHARED)
    SET(FIFECHAN_OPENGL_LIBRARY_TYPE STATIC)
  ENDIF(BUILD_FIFECHAN_OPENGL_SHARED)

  ADD_LIBRARY(${PROJECT_NAME}_opengl ${FIFECHAN_OPENGL_LIBRARY_TYPE} 

	${FIFECHAN_OPENGL_HEADER}
	${FIFECHAN_OPENGL_HEADERS}
	${FIFECHAN_OPENGL_CONTRIB_HEADERS}
	${FIFECHAN_OPENGL_SRC}
	${FIFECHAN_OPENGL_CONTRIB_SRC})

  IF(ENABLE_OPENGL_CONTRIB AND OGLFT_FOUND)
    TARGET_LINK_LIBRARIES(${PROJECT_NAME}_opengl ${OPENGL_LIBRARY} ${FREETYPE_LIBRARIES} ${OGLFT_LIBRARIES} ${PROJECT_NAME})
  ELSE(ENABLE_OPENGL_CONTRIB AND OGLFT_FOUND)
    TARGET_LINK_LIBRARIES(${PROJECT_NAME}_opengl ${OPENGL_LIBRARY} ${PROJECT_NAME})
  ENDIF(ENABLE_OPENGL_CONTRIB AND OGLFT_FOUND)

  ADD_CUSTOM_TARGET(opengllib DEPENDS ${PROJECT_NAME}_opengl) # Create symlink

  SET_TARGET_PROPERTIES(${PROJECT_NAME}_opengl PROPERTIES
    VERSION                 ${FIFECHAN_VERSION}
    SOVERSION               ${FIFECHAN_VERSION}
    CLEAN_DIRECT_OUTPUT     1                               # Allow creating static and shared libraries without conflict
    OUTPUT_NAME             ${PROJECT_NAME}_opengl          # Avoid conflicts between library and binary target names 
    COMPILE_DEFINITIONS     "FIFECHAN_EXTENSION_BUILD"
  )

  INSTALL(TARGETS ${PROJECT_NAME}_opengl DESTINATION lib${LIB_SUFFIX} PERMISSIONS
    OWNER_READ OWNER_WRITE OWNER_EXECUTE
    GROUP_READ GROUP_EXECUTE
    WORLD_READ WORLD_EXECUTE
  )

  INSTALL(FILES ${FIFECHAN_OPENGL_HEADER}           DESTINATION include/fifechan/)
  INSTALL(FILES ${FIFECHAN_OPENGL_HEADERS}          DESTINATION include/fifechan/opengl/)
  INSTALL(FILES ${FIFECHAN_OPENGL_CONTRIB_HEADERS}  DESTINATION include/fifechan/contrib/opengl/) 

ENDIF(ENABLE_OPENGL AND OPENGL_FOUND)

#------------------------------------------------------------------------------
#                   The Fifechan SDL extension library                                         
#------------------------------------------------------------------------------

FIND_PACKAGE(SDL2)
FIND_PACKAGE(SDL2_image)

IF(ENABLE_SDL_CONTRIB)
  FIND_PACKAGE(SDL2_ttf)
ENDIF(ENABLE_SDL_CONTRIB)

IF(APPLE AND ENABLE_SDL)
  FIND_LIBRARY(SDL2_LIBRARY SDL2)
  FIND_LIBRARY(SDL2_IMAGE_LIBRARY SDL2_image)
  MARK_AS_ADVANCED(SDL2_LIBRARY SDL2_IMAGE_LIBRARY)
  SET(EXTRA_LIBS ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARY})
ENDIF(APPLE AND ENABLE_SDL)

IF(ENABLE_SDL AND SDL2_FOUND AND SDL2_IMAGE_FOUND)  
 
  INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIR})
 
  # The Fifechan SDL extension source
  FILE(GLOB FIFECHAN_SDL_HEADER include/fifechan/sdl.hpp)
  FILE(GLOB FIFECHAN_SDL_HEADERS include/fifechan/sdl/*.hpp)
  FILE(GLOB FIFECHAN_SDL_CONTRIB_HEADERS include/fifechan/contrib/sdl/*.hpp)
  FILE(GLOB FIFECHAN_SDL_SRC src/sdl/*.cpp)

  IF(ENABLE_SDL_CONTRIB AND SDLTTF_FOUND)
    INCLUDE_DIRECTORIES(${SDL_TTF_INCLUDE_DIRS})
    FILE(GLOB FIFECHAN_CONTRIB_SRC src/contrib/sdl/*.cpp)  
  ENDIF(ENABLE_SDL_CONTRIB AND SDLTTF_FOUND)
  
  # Grouping of the source for nicer display in IDEs such as Visual Studio
  SOURCE_GROUP(src/fifechan               FILES ${FIFECHAN_SDL_HEADER})
  SOURCE_GROUP(src/fifechan/sdl           FILES ${FIFECHAN_SDL_HEADERS} ${FIFECHAN_SDL_SRC})
  SOURCE_GROUP(src/fifechan/contrib/sdl   FILES ${FIFECHAN_SDL_CONTRIB_HEADERS} ${FIFECHAN_CONTRIB_SRC})
 
  IF(BUILD_FIFECHAN_SDL_SHARED)
    SET(FIFECHAN_SDL_LIBRARY_TYPE SHARED)
  ELSE(BUILD_FIFECHAN_SDL_SHARED)
    SET(FIFECHAN_SDL_LIBRARY_TYPE STATIC)
  ENDIF(BUILD_FIFECHAN_SDL_SHARED)

  ADD_LIBRARY(${PROJECT_NAME}_sdl ${FIFECHAN_SDL_LIBRARY_TYPE} 
  	${FIFECHAN_SDL_HEADER}
  	${FIFECHAN_SDL_HEADERS}
  	${FIFECHAN_SDL_CONTRIB_HEADERS}
  	${FIFECHAN_SDL_SRC}
  	${FIFECHAN_CONTRIB_SRC}
  )

  IF(WIN32)
    get_filename_component(SDL2_LIBRARY_DIR ${SDL2_LIBRARY} DIRECTORY)

    IF(MINGW)
      TARGET_LINK_LIBRARIES(${PROJECT_NAME}_sdl ${MINGW32_LIBRARY} ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARY} ${SDL_TTF_LIBRARY} ${SDL2_LIBRARY_DIR}/SDL2main.lib ${PROJECT_NAME})
    ELSE(MINGW)
      TARGET_LINK_LIBRARIES(${PROJECT_NAME}_sdl ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARY} ${SDL_TTF_LIBRARY} ${SDL2_LIBRARY_DIR}/SDL2main.lib ${PROJECT_NAME})
    ENDIF(MINGW)
  ELSE(WIN32)
    TARGET_LINK_LIBRARIES(${PROJECT_NAME}_sdl ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARY} SDL2main ${PROJECT_NAME})
  ENDIF(WIN32)

  ADD_CUSTOM_TARGET(sdllib DEPENDS ${PROJECT_NAME}_sdl) # Create symlink

  SET_TARGET_PROPERTIES(${PROJECT_NAME}_sdl PROPERTIES
    VERSION                 ${FIFECHAN_VERSION}
    SOVERSION               ${FIFECHAN_VERSION}
    CLEAN_DIRECT_OUTPUT     1                               # Allow creating static and shared libraries without conflict
    OUTPUT_NAME             ${PROJECT_NAME}_sdl             # Avoid conflicts between library and binary target names
    COMPILE_DEFINITIONS     "FIFECHAN_EXTENSION_BUILD"
  )

  INSTALL(TARGETS ${PROJECT_NAME}_sdl DESTINATION lib${LIB_SUFFIX} PERMISSIONS
    OWNER_READ OWNER_WRITE OWNER_EXECUTE
    GROUP_READ GROUP_EXECUTE
    WORLD_READ WORLD_EXECUTE
  )

  INSTALL(FILES ${FIFECHAN_SDL_HEADER}            DESTINATION include/fifechan/)
  INSTALL(FILES ${FIFECHAN_SDL_HEADERS}           DESTINATION include/fifechan/sdl/)
  INSTALL(FILES ${FIFECHAN_SDL_CONTRIB_HEADERS}   DESTINATION include/fifechan/contrib/sdl/)   

ENDIF(ENABLE_SDL AND SDL2_FOUND AND SDL2_IMAGE_FOUND)

MESSAGE(STATUS "Fifechan Makefiles have been generated!")
