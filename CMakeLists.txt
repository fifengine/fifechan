#------------------------------------------------------------------------------
# FifeChan / FifeGUI
#------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.0...3.30)

# include additional CMake scripts
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake")

#-------------------------------------------------------------------
# Setup VCPKG and install Dependencies
#-------------------------------------------------------------------

# This configures VCPKG to install the dependencies defined in vcpkg.json
# autmatically during the configure step of CMake (vcpkg install).
include(SetupVcpkg)

#-------------------------------------------------------------------
# Version
#-------------------------------------------------------------------

# vcpkg.json is the primary source for version data
file(READ ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg.json VCPKG_JSON_STRING)
string(JSON FIFECHAN_VERSION GET ${VCPKG_JSON_STRING} "version")

#-------------------------------------------------------------------
# Project
#-------------------------------------------------------------------

project(fifechan VERSION ${FIFECHAN_VERSION} LANGUAGES CXX)

#-------------------------------------------------------------------
# CMake Includes
#-------------------------------------------------------------------

# Project local includes
include(ShowBuildTargetProperties)

# CMake provided includes
include(FeatureSummary)
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

#------------------------------------------------------------------------------
# Options
#------------------------------------------------------------------------------

option(BUILD_SHARED_LIBS     "Build using shared libraries"                     OFF)

option(ENABLE_ALLEGRO        "Enable the Fifechan Allegro extension"            OFF)
option(ENABLE_IRRLICHT       "Enable the Fifechan Irrlicht extension"           OFF) # removed from vcpkg, doesnt compile with C++17
option(ENABLE_OPENGL         "Enable the Fifechan OpenGL extension"             ON)
option(ENABLE_SDL            "Enable the Fifechan SDL extension"                ON)

option(FIFECHAN_TESTS        "Build the Fifechan tests"                         ON)
option(FIFECHAN_EXAMPLES     "Build the Fifechan examples"                      OFF)

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

# On some platforms, we need to append a suffix to the lib folder.
# You can set this to 64 to get "lib64" or to an empty string to get "lib".
set(LIB_SUFFIX "" CACHE STRING "Suffix to append to the system `lib` folder, e.g. '64'")

#------------------------------------------------------------------------------
# Display Status
#------------------------------------------------------------------------------

message(STATUS "---------------------------------------------------------------")
message(STATUS "[ 🛠️ ] Building FIFECHAN version ${FIFECHAN_VERSION} using build type '${CMAKE_BUILD_TYPE}'.")
message(STATUS "---------------------------------------------------------------")
message(STATUS "[INFO] Source dir              -> '${PROJECT_SOURCE_DIR}'.")
message(STATUS "[INFO] Build dir               -> '${PROJECT_BINARY_DIR}'.")
message(STATUS "[INFO] Installation path       -> '${CMAKE_INSTALL_PREFIX}' (set -DCMAKE_INSTALL_PREFIX=<path> to change).")

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
# Fifechan core library
#------------------------------------------------------------------------------

add_library(${PROJECT_NAME} ${FIFECHAN_LIBRARY_BUILD_TYPE}
    ${FIFECHAN_HEADER}
    ${FIFECHAN_HEADERS}
    ${FIFECHAN_WIDGET_HEADERS}
    ${FIFECHAN_UTIL_HEADERS}
    ${FIFECHAN_SRC}
    ${FIFECHAN_WIDGET_SRC}
)

# Provide a namespace alias for the library (fifechan::fifechan)
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
 )

set_target_properties(${PROJECT_NAME} PROPERTIES
  VERSION                ${FIFECHAN_VERSION}
  SOVERSION              ${FIFECHAN_VERSION}
  CLEAN_DIRECT_OUTPUT    1                        # Allow creating static and shared libraries without conflict
  OUTPUT_NAME            ${PROJECT_NAME}          # Avoid conflicts between library and binary target names
  COMPILE_DEFINITIONS    "FIFECHAN_BUILD"
  CXX_STANDARD           17
  CXX_STANDARD_REQUIRED  ON
)

show_build_target_properties(${PROJECT_NAME})

install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}Targets DESTINATION lib${LIB_SUFFIX})

install(FILES ${FIFECHAN_HEADER}          DESTINATION include/)
install(FILES ${FIFECHAN_HEADERS}         DESTINATION include/fifechan/)
install(FILES ${FIFECHAN_WIDGET_HEADERS}  DESTINATION include/fifechan/widgets/)
install(FILES ${FIFECHAN_UTIL_HEADERS}    DESTINATION include/fifechan/util/)

#------------------------------------------------------------------------------
# Fifechan Allegro extension library
#------------------------------------------------------------------------------

if(ENABLE_ALLEGRO)
  find_package(Allegro)
endif()

if(ALLEGRO_FOUND)

  # The Fifechan Allegro extension source
  file(GLOB FIFECHAN_ALLEGRO_HEADER     include/fifechan/allegro.hpp)
  file(GLOB FIFECHAN_ALLEGRO_HEADERS    include/fifechan/allegro/*.hpp)
  file(GLOB FIFECHAN_ALLEGRO_SRC        src/allegro/*.cpp)

  add_library(${PROJECT_NAME}_allegro ${FIFECHAN_LIBRARY_BUILD_TYPE}
    ${FIFECHAN_ALLEGRO_HEADER}
    ${FIFECHAN_ALLEGRO_HEADERS}
    ${FIFECHAN_ALLEGRO_SRC}
  )

  # Provide a namespace alias for the library (fifechan::fifechan_allegro)
  add_library(${PROJECT_NAME}::${PROJECT_NAME}_allegro ALIAS ${PROJECT_NAME}_allegro)

  target_include_directories(${PROJECT_NAME}_allegro PUBLIC
    $<BUILD_INTERFACE:${ALLEGRO_INCLUDE_DIR}>
    $<INSTALL_INTERFACE:include>
  )

  target_link_libraries(${PROJECT_NAME}_allegro PRIVATE
   #${ALLEG_LIBRARY}
   # Allegro
   Allegro::allegro Allegro::allegro_ttf Allegro::allegro_font Allegro::allegro_main
   # Fifechan
   ${PROJECT_NAME}
  )

  set_target_properties(${PROJECT_NAME}_allegro PROPERTIES
    VERSION                ${FIFECHAN_VERSION}
    SOVERSION              ${FIFECHAN_VERSION}
    CLEAN_DIRECT_OUTPUT    1                               # Allow creating static and shared libraries without conflict
    OUTPUT_NAME            ${PROJECT_NAME}_allegro         # Avoid conflicts between library and binary target names
    COMPILE_DEFINITIONS    "FIFECHAN_EXTENSION_BUILD"
    CXX_STANDARD           17
    CXX_STANDARD_REQUIRED  ON
  )

  show_build_target_properties(${PROJECT_NAME}_allegro)

  install(TARGETS ${PROJECT_NAME}_allegro EXPORT ${PROJECT_NAME}Targets DESTINATION lib${LIB_SUFFIX})

  install(FILES ${FIFECHAN_ALLEGRO_HEADER}   DESTINATION include/fifechan/)
  install(FILES ${FIFECHAN_ALLEGRO_HEADERS}  DESTINATION include/fifechan/allegro/)

endif()

#------------------------------------------------------------------------------
# Fifechan Irrlicht extension library
#------------------------------------------------------------------------------

if(ENABLE_IRRLICHT)
  find_package(Irrlicht)
endif()

if(IRRLICHT_FOUND)

  # The Fifechan Irrlicht extension source
  file(GLOB FIFECHAN_IRRLICHT_HEADER    include/fifechan/irrlicht.hpp)
  file(GLOB FIFECHAN_IRRLICHT_HEADERS   include/fifechan/irrlicht/*.hpp)
  file(GLOB FIFECHAN_IRRLICHT_SRC       src/irrlicht/*.cpp)

  add_library(${PROJECT_NAME}_irrlicht ${FIFECHAN_LIBRARY_BUILD_TYPE}
    ${FIFECHAN_IRRLICHT_HEADER}
    ${FIFECHAN_IRRLICHT_HEADERS}
    ${FIFECHAN_IRRLICHT_SRC}
  )

  # Provide a namespace alias for the library (fifechan::fifechan_allegro)
  add_library(${PROJECT_NAME}::${PROJECT_NAME}_irrlicht ALIAS ${PROJECT_NAME}_allegro)

  target_include_directories(${PROJECT_NAME}_irrlicht PUBLIC
    $<BUILD_INTERFACE:${IRRLICHT_INCLUDE_DIR}>
    $<INSTALL_INTERFACE:include>
  )

  target_link_libraries(${PROJECT_NAME}_irrlicht PRIVATE ${IRRLICHT_LIBRARY} ${PROJECT_NAME})

  set_target_properties(${PROJECT_NAME}_irrlicht PROPERTIES
    VERSION                ${FIFECHAN_VERSION}
    SOVERSION              ${FIFECHAN_VERSION}
    CLEAN_DIRECT_OUTPUT    1                               # Allow creating static and shared libraries without conflict
    OUTPUT_NAME            ${PROJECT_NAME}_irrlicht        # Avoid conflicts between library and binary target names
    COMPILE_DEFINITIONS    "FIFECHAN_EXTENSION_BUILD"
    CXX_STANDARD           17
    CXX_STANDARD_REQUIRED  ON
  )

  show_build_target_properties(${PROJECT_NAME}_irrlicht)

  install(TARGETS ${PROJECT_NAME}_irrlicht EXPORT ${PROJECT_NAME}Targets DESTINATION lib${LIB_SUFFIX})

  install(FILES ${FIFECHAN_IRRLICHT_HEADER}   DESTINATION include/fifechan/)
  install(FILES ${FIFECHAN_IRRLICHT_HEADERS}  DESTINATION include/fifechan/irrlicht/)

endif()

#------------------------------------------------------------------------------
# The Fifechan OpenGL extension library
#------------------------------------------------------------------------------

if(ENABLE_OPENGL)
  find_package(OpenGL)
  find_package(Freetype REQUIRED)

  if(APPLE)
    find_library(OPENGL_LIBRARY OpenGL)
    mark_as_advanced(OPENGL_LIBRARY)
    set(EXTRA_LIBS ${OPENGL_LIBRARY})
  endif()
endif()

if(OPENGL_FOUND)

  # set extension sources
  file(GLOB FIFECHAN_OPENGL_HEADER  include/fifechan/opengl.hpp)
  file(GLOB FIFECHAN_OPENGL_HEADERS include/fifechan/opengl/*.hpp)
  file(GLOB FIFECHAN_OPENGL_SRC     src/opengl/*.cpp)

  # create the extension library
  add_library(${PROJECT_NAME}_opengl ${FIFECHAN_LIBRARY_BUILD_TYPE}
    ${FIFECHAN_OPENGL_HEADER}
    ${FIFECHAN_OPENGL_HEADERS}
    ${FIFECHAN_OPENGL_SRC}
  )

  # Provide a namespace alias for the library (fifechan::fifechan_opengl)
  add_library(${PROJECT_NAME}::${PROJECT_NAME}_opengl ALIAS ${PROJECT_NAME}_opengl)

  # add include directories for the dependencies of the extension
  target_include_directories(${PROJECT_NAME}_opengl PUBLIC
    $<BUILD_INTERFACE:${OPENGL_INCLUDE_DIR}>
    $<BUILD_INTERFACE:${FREETYPE_INCLUDE_DIRS}>
    $<INSTALL_INTERFACE:include>
  )

  # link dependencies to the extension
  target_link_libraries(${PROJECT_NAME}_opengl PRIVATE ${OPENGL_LIBRARY} Freetype::Freetype ${PROJECT_NAME})

  # set additional properties for the extension
  set_target_properties(${PROJECT_NAME}_opengl PROPERTIES
    VERSION                ${FIFECHAN_VERSION}
    SOVERSION              ${FIFECHAN_VERSION}
    CLEAN_DIRECT_OUTPUT    1                               # Allow creating static and shared libraries without conflict
    OUTPUT_NAME            ${PROJECT_NAME}_opengl          # Avoid conflicts between library and binary target names
    COMPILE_DEFINITIONS    "FIFECHAN_EXTENSION_BUILD"
    CXX_STANDARD           17
    CXX_STANDARD_REQUIRED  ON
  )

  # print overview
  show_build_target_properties(${PROJECT_NAME}_opengl)

  # setup install location
  install(TARGETS ${PROJECT_NAME}_opengl EXPORT ${PROJECT_NAME}Targets DESTINATION lib${LIB_SUFFIX})

  # setup additional files to install
  install(FILES ${FIFECHAN_OPENGL_HEADER}   DESTINATION include/fifechan/)
  install(FILES ${FIFECHAN_OPENGL_HEADERS}  DESTINATION include/fifechan/opengl/)

endif()

#------------------------------------------------------------------------------
# The Fifechan SDL extension library
#------------------------------------------------------------------------------

if(ENABLE_SDL)
  find_package(SDL2 CONFIG REQUIRED)
  find_package(SDL2_image CONFIG REQUIRED)
  find_package(SDL2_ttf CONFIG REQUIRED)

  if(APPLE)
    find_library(SDL2_LIBRARY SDL2)
    find_library(SDL2_IMAGE_LIBRARY SDL2_image)
    mark_as_advanced(SDL2_LIBRARY SDL2_IMAGE_LIBRARY)
    set(EXTRA_LIBS ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARY})
  endif()
endif()

if(SDL2_FOUND AND SDL2_image_FOUND)

  # The Fifechan SDL extension source
  file(GLOB FIFECHAN_SDL_HEADER     include/fifechan/sdl.hpp)
  file(GLOB FIFECHAN_SDL_HEADERS    include/fifechan/sdl/*.hpp)
  file(GLOB FIFECHAN_SDL_SRC        src/sdl/*.cpp)

  add_library(${PROJECT_NAME}_sdl ${FIFECHAN_LIBRARY_BUILD_TYPE}
    ${FIFECHAN_SDL_HEADER}
    ${FIFECHAN_SDL_HEADERS}
    ${FIFECHAN_SDL_SRC}
  )

  # Provide a namespace alias for the library (fifechan::fifechan_sdl)
  add_library(${PROJECT_NAME}::${PROJECT_NAME}_sdl ALIAS ${PROJECT_NAME}_sdl)

  target_include_directories(${PROJECT_NAME}_sdl PUBLIC
    $<BUILD_INTERFACE:${SDL2_INCLUDE_DIRS}>
    $<BUILD_INTERFACE:${SDL2_IMAGE_INCLUDE_DIR}>
    $<BUILD_INTERFACE:${SDL2_TTF_INCLUDE_DIRS}>
    $<INSTALL_INTERFACE:include>
  )

  target_link_libraries(${PROJECT_NAME}_sdl PRIVATE
    # SDL2::SDL2
    $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
    $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
    # SDL2::SDL2_image
    $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>
    # SDL2::SDL2_ttf
    $<IF:$<TARGET_EXISTS:SDL2_ttf::SDL2_ttf>,SDL2_ttf::SDL2_ttf,SDL2_ttf::SDL2_ttf-static>
    # Fifechan
    ${PROJECT_NAME}
  )

  set_target_properties(${PROJECT_NAME}_sdl PROPERTIES
    VERSION                ${FIFECHAN_VERSION}
    SOVERSION              ${FIFECHAN_VERSION}
    CLEAN_DIRECT_OUTPUT    1                               # Allow creating static and shared libraries without conflict
    OUTPUT_NAME            ${PROJECT_NAME}_sdl             # Avoid conflicts between library and binary target names
    COMPILE_DEFINITIONS    "FIFECHAN_EXTENSION_BUILD"
    CXX_STANDARD           17
    CXX_STANDARD_REQUIRED  ON
  )

  install(TARGETS ${PROJECT_NAME}_sdl EXPORT ${PROJECT_NAME}Targets DESTINATION lib${LIB_SUFFIX})

  install(FILES ${FIFECHAN_SDL_HEADER}   DESTINATION include/fifechan/)
  install(FILES ${FIFECHAN_SDL_HEADERS}  DESTINATION include/fifechan/sdl/)

endif()

#------------------------------------------------------------------------------
# Examples
#------------------------------------------------------------------------------

if(FIFECHAN_EXAMPLES)
  add_subdirectory(examples)
endif()

#------------------------------------------------------------------------------
# Tests
#------------------------------------------------------------------------------

if(FIFECHAN_TESTS)
  # enable unit tests
  #enable_testing()
  #add_subdirectory(tests/unit)

  # enable integration tests
  add_subdirectory("tests/integration")
endif()

#------------------------------------------------------------------------------
# Display Summary
#------------------------------------------------------------------------------

feature_summary(WHAT PACKAGES_FOUND PACKAGES_NOT_FOUND ENABLED_FEATURES DISABLED_FEATURES)

#------------------------------------------------------------------------------
# Installation Rules
#------------------------------------------------------------------------------
# 1) Generate and install target configuration files (for use with find_package)
# 2) Generate and install package configuration files (for find_package)
# 3) Generate and install a pkg-config file (for pkg-config)

#------------------------------------------------------------------------------
# 1 Generate and install target configuration files (for use with find_package)
#------------------------------------------------------------------------------

install(EXPORT ${PROJECT_NAME}Targets
        FILE ${PROJECT_NAME}Targets.cmake
        NAMESPACE ${PROJECT_NAME}::
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
)

#------------------------------------------------------------------------------
# 2 Generate and install package configuration files (for find_package)
#------------------------------------------------------------------------------

# fifechanConfig.cmake
configure_package_config_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/CMake/packaging/fifechanConfig.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/fifechanConfig.cmake"
  INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
)

# fifechanConfigVersion.cmake
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/fifechanConfigVersion.cmake"
  COMPATIBILITY SameMajorVersion
)

install(FILES
  "${CMAKE_CURRENT_BINARY_DIR}/fifechanConfig.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/fifechanConfigVersion.cmake"
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/fifechan"
)

#------------------------------------------------------------------------------
# 3 Generate and install a pkg-config file (for pkg-config)
#------------------------------------------------------------------------------

# fifechan.pc
configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/CMake/packaging/fifechan.pc.in"
  "${CMAKE_CURRENT_BINARY_DIR}/fifechan.pc"
  @ONLY
)

install(FILES
  "${CMAKE_CURRENT_BINARY_DIR}/fifechan.pc"
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig"
)
