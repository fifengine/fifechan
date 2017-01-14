#------------------------------------------------------------------------------
#                       FifeChan / FifeGUI Dependencies                                               
#------------------------------------------------------------------------------

CMAKE_MINIMUM_REQUIRED(VERSION 3.5)

PROJECT(fifechan-dependencies)

set(CMAKE_INSTALL_PREFIX    ${CMAKE_SOURCE_DIR}/../../${PROJECT_NAME}/includes)
set(CMAKE_MODULE_PATH       ${CMAKE_SOURCE_DIR}/../cmake)
set(CMAKE_VERBOSE_MAKEFILE  ON)

if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set (CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}" CACHE PATH "default install path" FORCE )
endif()

message(STATUS "\nWelcome!\n== Build System for ${PROJECT_NAME}")

# create a "Release" build by default
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING
    "Select build type. Options are: 'Debug', 'Release', 'RelWithDebInfo' and 'MinSizeRel'."
    FORCE
  )  
  message(STATUS "Set CMAKE_BUILD_TYPE to default '${CMAKE_BUILD_TYPE}'.")
endif(NOT CMAKE_BUILD_TYPE)

# set a fallback download folder for dependencies ("dependencies/../../${PROJECT_NAME}/downloads")
if(NOT DEPENDENCY_DOWNLOAD_DIR)
    set(DEPENDENCY_DOWNLOAD_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../${PROJECT_NAME}/downloads)
    message("   DEPENDENCY_DOWNLOAD_DIR not set. Using default.")
endif(NOT DEPENDENCY_DOWNLOAD_DIR)

# set a fallback installation folder for dependencies ("dependencies/../../${PROJECT_NAME}/includes")
if(NOT DEPENDENCY_INSTALL_DIR)
    set(DEPENDENCY_INSTALL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../${PROJECT_NAME}/includes)
    message("   DEPENDENCY_INSTALL_DIR not set. Using default.")    
endif(NOT DEPENDENCY_INSTALL_DIR)

# set a fallback folder for the extracted dependencies ("dependencies/../../${PROJECT_NAME}/extracted")
if(NOT DEPENDENCY_EXTRACT_DIR)
    set(DEPENDENCY_EXTRACT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../${PROJECT_NAME}/extracted)
    message("   DEPENDENCY_EXTRACT_DIR not set. Using default.")    
endif(NOT DEPENDENCY_EXTRACT_DIR)

#------------------------------------------------------------------------------
#                                 Display Status                                         
#------------------------------------------------------------------------------

message("")
message("== Config Overview")
message("")
message("   CMAKE_BUILD_TYPE           |  ${CMAKE_BUILD_TYPE}")
message("   CMAKE_BINARY_DIR           |  ${CMAKE_BINARY_DIR}")
message("   CMAKE_INSTALL_PREFIX       |  ${CMAKE_INSTALL_PREFIX}")
message("   DEPENDENCY_INSTALL_DIR     |  ${DEPENDENCY_INSTALL_DIR}") 
message("   DEPENDENCY_DOWNLOAD_DIR    |  ${DEPENDENCY_DOWNLOAD_DIR}") 
message("   DEPENDENCY_EXTRACT_DIR     |  ${DEPENDENCY_EXTRACT_DIR}") 
message("")

#------------------------------------------------------------------------------
#                                Options                                                   
#------------------------------------------------------------------------------

OPTION(FIFECHAN_DEPENDENCY_SDL2        "Download and build dependency: SDL2"       ON)
OPTION(FIFECHAN_DEPENDENCY_SDL2_IMAGE  "Download and build dependency: SDL2_IMAGE" ON)
OPTION(FIFECHAN_DEPENDENCY_SDL2_TTF    "Download and build dependency: SDL2_TTF"   ON)
OPTION(FIFECHAN_DEPENDENCY_ALLEGRO     "Download and build dependency: ALLEGRO"    OFF)
OPTION(FIFECHAN_DEPENDENCY_IRRLICHT    "Download and build dependency: IRRLICHT"   OFF)
OPTION(FIFECHAN_DEPENDENCY_OGLFT       "Download and build dependency: OGLFT"      OFF)

#------------------------------------------------------------------------------
#                   Check Options + Include external build scripts                                             
#------------------------------------------------------------------------------

IF(FIFECHAN_DEPENDENCY_SDL2)
    INCLUDE(get-sdl2)
ENDIF(FIFECHAN_DEPENDENCY_SDL2)

IF(FIFECHAN_DEPENDENCY_SDL2_IMAGE)
    INCLUDE(get-sdl2_image)
ENDIF(FIFECHAN_DEPENDENCY_SDL2_IMAGE)

IF(FIFECHAN_DEPENDENCY_SDL2_TTF)
    INCLUDE(get-sdl2_ttf)
ENDIF(FIFECHAN_DEPENDENCY_SDL2_TTF)

IF(FIFECHAN_DEPENDENCY_ALLEGRO)
    INCLUDE(get-allegro)
ENDIF(FIFECHAN_DEPENDENCY_ALLEGRO)

IF(FIFECHAN_DEPENDENCY_IRRLICHT)
    INCLUDE(get-irrlicht)
ENDIF(FIFECHAN_DEPENDENCY_IRRLICHT)

IF(FIFECHAN_DEPENDENCY_OGLFT)
    INCLUDE(get-oglft)
ENDIF(FIFECHAN_DEPENDENCY_OGLFT)
