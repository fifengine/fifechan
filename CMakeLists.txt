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
# automatically during the configure step of CMake (vcpkg install).
include(SetupVcpkg)

#-------------------------------------------------------------------
# Gather Version Data
#-------------------------------------------------------------------

# vcpkg.json is the primary source for version data.
# FIFECHAN_VERSION is used to set the project version.
# FIFECHAN_VERSION and the other tokens are used in version.hpp.in.
file(READ ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg.json VCPKG_JSON_STRING)
string(JSON FIFECHAN_NAME     GET ${VCPKG_JSON_STRING} "name")
string(JSON FIFECHAN_VERSION  GET ${VCPKG_JSON_STRING} "version")
string(JSON FIFECHAN_LICENSE  GET ${VCPKG_JSON_STRING} "license")
string(JSON FIFECHAN_DESC     GET ${VCPKG_JSON_STRING} "description")
string(JSON FIFECHAN_HOMEPAGE GET ${VCPKG_JSON_STRING} "homepage")
string(TIMESTAMP CURRENT_YEAR "%Y")

#-------------------------------------------------------------------
# Project
#-------------------------------------------------------------------

project(fifechan VERSION ${FIFECHAN_VERSION} LANGUAGES CXX)

#-------------------------------------------------------------------
# Versionize the project
#-------------------------------------------------------------------

# Here we populate the version.hpp file with version data.
# This must be done after project() is called,
# else the version variables are not available (PROJECT_* tokens).
# See the version.hpp.in file for the tokens that are replaced.
configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/include/fifechan/version.hpp.in"
  "${CMAKE_CURRENT_BINARY_DIR}/include/fifechan/version.hpp"
  @ONLY
)

#-------------------------------------------------------------------
# CMake Includes
#-------------------------------------------------------------------

# Project local includes
include(ShowBuildTargetProperties)
include(CopyResources)

# CMake provided includes
include(FeatureSummary)
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

if(UNIX AND NOT APPLE)
  set(CMAKE_INSTALL_RPATH "\$ORIGIN;\$ORIGIN/../${CMAKE_INSTALL_LIBDIR}")
  set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
endif()

#------------------------------------------------------------------------------
# Options
#------------------------------------------------------------------------------

option(BUILD_SHARED_LIBS     "Build using shared libraries"                     OFF)

option(ENABLE_OPENGL         "Enable the Fifechan OpenGL extension"             ON)
option(ENABLE_SDL            "Enable the Fifechan SDL extension"                ON)

option(FIFECHAN_TESTS        "Build the Fifechan tests"                         ON)
option(FIFECHAN_EXAMPLES     "Build the Fifechan examples"                      OFF)

#------------------------------------------------------------------------------
# Default build type
#------------------------------------------------------------------------------

# set the default build type to 'Debug' if not specified
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting default CMAKE_BUILD_TYPE to 'Debug'")
  set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui, ccmake
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

# The default build type is based on BUILD_SHARED_LIBS.
# FIFECHAN_LIBRARY_BUILD_TYPE is used on add_library() calls.
if(BUILD_SHARED_LIBS)
  set(FIFECHAN_LIBRARY_BUILD_TYPE SHARED)
else()
  set(FIFECHAN_LIBRARY_BUILD_TYPE STATIC)
endif()

#------------------------------------------------------------------------------
# Display Status
#------------------------------------------------------------------------------

message(STATUS "---------------------------------------------------------------")
message(STATUS "[ 🛠️ ] Building FIFECHAN version ${FIFECHAN_VERSION} using build type '${CMAKE_BUILD_TYPE}'.")
message(STATUS "---------------------------------------------------------------")
message(STATUS "[INFO] Source dir               -> '${PROJECT_SOURCE_DIR}'.")
message(STATUS "[INFO] Build dir                -> '${PROJECT_BINARY_DIR}'.")
message(STATUS "[INFO] Installation path        -> '${CMAKE_INSTALL_PREFIX}' (set -DCMAKE_INSTALL_PREFIX=<path> to change).")
message(STATUS "")
message(STATUS "[INFO] Build shared libraries   -> '${BUILD_SHARED_LIBS}'.")
message(STATUS "[INFO] Enable OpenGL extension  -> '${ENABLE_OPENGL}'.")
message(STATUS "[INFO] Enable SDL extension     -> '${ENABLE_SDL}'.")
message(STATUS "[INFO] Build tests              -> '${FIFECHAN_TESTS}'.")
message(STATUS "[INFO] Build examples           -> '${FIFECHAN_EXAMPLES}'.")
message(STATUS "---------------------------------------------------------------")

#------------------------------------------------------------------------------
# Configure
#------------------------------------------------------------------------------

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

# This shows the invoked commands during makefile builds. Only for debugging.
#set(CMAKE_VERBOSE_MAKEFILE ON CACHE BOOL "ON")

# Enable color diagnostics (-fcolor-diagnostics).
SET(CMAKE_COLOR_DIAGNOSTICS ON)

#------------------------------------------------------------------------------
# Compiler Options
#------------------------------------------------------------------------------

if(WIN32)
  if(MSVC)
    # ensure we use minimal "windows.h" lib without the crazy min max macros
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /D \"WIN32_LEAN_AND_MEAN\"")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /D \"NOMINMAX\"")
  endif()
else()
  # disable C++20 module support
  set(CMAKE_CXX_SCAN_FOR_MODULES 0)

  # Unix-like systems (Linux, macOS, etc.)
  if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    # Use libc++ for Clang
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -stdlib=libc++")
  elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    message(STATUS "Using GCC ${CMAKE_CXX_COMPILER_VERSION} with libstdc++")
  endif()
endif()

#------------------------------------------------------------------------------
# Source Files for Fifechan core library
#------------------------------------------------------------------------------

file(GLOB FIFECHAN_HEADER          include/fifechan.hpp)
file(GLOB FIFECHAN_HEADERS         include/fifechan/*.hpp)
file(GLOB FIFECHAN_WIDGET_HEADERS  include/fifechan/widgets/*.hpp)
file(GLOB FIFECHAN_UTIL_HEADERS    include/fifechan/util/*.hpp)
file(GLOB FIFECHAN_SRC             src/*.cpp)
file(GLOB FIFECHAN_WIDGET_SRC      src/widgets/*.cpp)
set(FIFECHAN_VERSION_HEADER        "${CMAKE_CURRENT_BINARY_DIR}/include/fifechan/version.hpp")

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
    ${FIFECHAN_VERSION_HEADER}
)

# Provide a namespace alias for the library (fifechan::fifechan)
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include> # to include the generated "version.hpp"
    $<INSTALL_INTERFACE:include>
)

set_target_properties(${PROJECT_NAME} PROPERTIES
  VERSION                ${FIFECHAN_VERSION}
  SOVERSION              ${FIFECHAN_VERSION}
  CLEAN_DIRECT_OUTPUT    1                        # Allow creating static and shared libraries without conflict
  OUTPUT_NAME            ${PROJECT_NAME}          # Avoid conflicts between library and binary target names
  COMPILE_DEFINITIONS    "FIFECHAN_BUILD"
  LINKER_LANGUAGE        CXX
  CXX_STANDARD           20
  CXX_STANDARD_REQUIRED  ON
)

show_build_target_properties(${PROJECT_NAME})

install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}-targets
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  PERMISSIONS
    OWNER_READ OWNER_WRITE OWNER_EXECUTE
    GROUP_READ GROUP_EXECUTE
    WORLD_READ WORLD_EXECUTE
)

install(FILES ${FIFECHAN_HEADER}          DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/)
install(FILES ${FIFECHAN_HEADERS}         DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/fifechan/)
install(FILES ${FIFECHAN_WIDGET_HEADERS}  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/fifechan/widgets/)
install(FILES ${FIFECHAN_UTIL_HEADERS}    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/fifechan/util/)

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
  file(GLOB FIFECHAN_OPENGL_HEADERS include/fifechan/backends/opengl/*.hpp)
  file(GLOB FIFECHAN_OPENGL_SRC     src/backends/opengl/*.cpp)

  # create the extension library
  add_library(${PROJECT_NAME}_opengl ${FIFECHAN_LIBRARY_BUILD_TYPE}
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
    LINKER_LANGUAGE        CXX
    CXX_STANDARD           20
    CXX_STANDARD_REQUIRED  ON
  )

  # print overview
  show_build_target_properties(${PROJECT_NAME}_opengl)

  # setup install location
  install(TARGETS ${PROJECT_NAME}_opengl EXPORT ${PROJECT_NAME}-targets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PERMISSIONS
      OWNER_READ OWNER_WRITE OWNER_EXECUTE
      GROUP_READ GROUP_EXECUTE
      WORLD_READ WORLD_EXECUTE
  )

  # setup additional files to install
  install(FILES ${FIFECHAN_OPENGL_HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/fifechan/backends/opengl/)

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
  file(GLOB FIFECHAN_SDL_HEADERS    include/fifechan/backends/sdl2/*.hpp)
  file(GLOB FIFECHAN_SDL_SRC        src/backends/sdl2/*.cpp)

  add_library(${PROJECT_NAME}_sdl2 ${FIFECHAN_LIBRARY_BUILD_TYPE}
    ${FIFECHAN_SDL_HEADERS}
    ${FIFECHAN_SDL_SRC}
  )

  # Provide a namespace alias for the library (fifechan::fifechan_sdl2)
  add_library(${PROJECT_NAME}::${PROJECT_NAME}_sdl2 ALIAS ${PROJECT_NAME}_sdl2)

  target_include_directories(${PROJECT_NAME}_sdl2 PUBLIC
    # Only set the SDL2 include dir, SDL2_image and SDL2_ttf are included in the SDL2 include directory.
    $<BUILD_INTERFACE:${SDL2_INCLUDE_DIRS}>
    $<INSTALL_INTERFACE:include>
  )

  target_link_libraries(${PROJECT_NAME}_sdl2 PRIVATE
    # SDL2::SDL2
    #$<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
    $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
    # SDL2::SDL2_image
    $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>
    # SDL2::SDL2_ttf
    $<IF:$<TARGET_EXISTS:SDL2_ttf::SDL2_ttf>,SDL2_ttf::SDL2_ttf,SDL2_ttf::SDL2_ttf-static>
    # Fifechan
    ${PROJECT_NAME}
  )

  set_target_properties(${PROJECT_NAME}_sdl2 PROPERTIES
    VERSION                ${FIFECHAN_VERSION}
    SOVERSION              ${FIFECHAN_VERSION}
    CLEAN_DIRECT_OUTPUT    1                               # Allow creating static and shared libraries without conflict
    OUTPUT_NAME            ${PROJECT_NAME}_sdl2            # Avoid conflicts between library and binary target names
    COMPILE_DEFINITIONS    "FIFECHAN_EXTENSION_BUILD"
    LINKER_LANGUAGE        CXX
    CXX_STANDARD           20
    CXX_STANDARD_REQUIRED  ON
  )

  install(TARGETS ${PROJECT_NAME}_sdl2 EXPORT ${PROJECT_NAME}-targets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PERMISSIONS
      OWNER_READ OWNER_WRITE OWNER_EXECUTE
      GROUP_READ GROUP_EXECUTE
      WORLD_READ WORLD_EXECUTE
  )

  install(FILES ${FIFECHAN_SDL_HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/fifechan/backends/sdl2/)

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

install(EXPORT ${PROJECT_NAME}-targets
        FILE ${PROJECT_NAME}-targets.cmake
        NAMESPACE ${PROJECT_NAME}::
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
)

#------------------------------------------------------------------------------
# 2 Generate and install package configuration files (for find_package)
#------------------------------------------------------------------------------

# fifechan-config.cmake
configure_package_config_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/CMake/packaging/fifechan-config.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/fifechan-config.cmake"
  INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
)

# fifechan-config-version.cmake
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/fifechan-config-version.cmake"
  COMPATIBILITY SameMajorVersion
)

install(FILES
  "${CMAKE_CURRENT_BINARY_DIR}/fifechan-config.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/fifechan-config-version.cmake"
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
