#------------------------------------------------------------------------------
# FifeGUI (formerly FifeChan)
#------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.25...4.2)

# MSVC runtime library flags are selected by an abstraction.
# In short: do not ignore CMAKE_MSVC_RUNTIME_LIBRARY.
# https://cmake.org/cmake/help/latest/policy/CMP0091.html
if(POLICY CMP0091)
  cmake_policy(SET CMP0091 NEW)
endif()

# include additional CMake scripts
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake")

#-------------------------------------------------------------------
# Setup VCPKG and install Dependencies
#-------------------------------------------------------------------

# USE_VCPKG defaults to true, so that SetupVcpkg is included.
set(USE_VCPKG TRUE CACHE BOOL "Use vcpkg for dependencies")

# This configures VCPKG to install the dependencies defined in vcpkg.json
# automatically during the configure step of CMake (vcpkg install).
if(USE_VCPKG)
   include(SetupVcpkg)
endif()

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

# Enable C so transitive dependencies using FindThreads can detect pthreads.
project(fifechan VERSION ${FIFECHAN_VERSION} LANGUAGES C CXX)

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
include(InstallExample)

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

option(BUILD_SHARED_LIBS     "Build using shared libraries"                 OFF)

option(ENABLE_OPENGL         "Enable the FifeGUI OpenGL extension"           ON)
option(ENABLE_SDL            "Enable the FifeGUI SDL extension"              ON)

option(FIFEGUI_TESTS         "Build the FifeGUI tests"                       ON)
option(FIFEGUI_EXAMPLES      "Build the FifeGUI examples"                    ON)

# Reminder: The variable USE_VCPKG exist, which controls SetupVcpkg.
# It is not a build option, because it must be set before project().

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
message(STATUS "[ 🛠️ ] Building FIFEGUI version ${FIFECHAN_VERSION} using build type '${CMAKE_BUILD_TYPE}'.")
message(STATUS "---------------------------------------------------------------")
message(STATUS "[INFO] Source dir               -> '${PROJECT_SOURCE_DIR}'.")
message(STATUS "[INFO] Build dir                -> '${PROJECT_BINARY_DIR}'.")
message(STATUS "[INFO] Installation path        -> '${CMAKE_INSTALL_PREFIX}' (set -DCMAKE_INSTALL_PREFIX=<path> to change).")
message(STATUS "")
message(STATUS "[INFO] Build shared libraries   -> '${BUILD_SHARED_LIBS}'.")
message(STATUS "[INFO] Enable OpenGL extension  -> '${ENABLE_OPENGL}'.")
message(STATUS "[INFO] Enable SDL extension     -> '${ENABLE_SDL}'.")
message(STATUS "[INFO] Build tests              -> '${FIFEGUI_TESTS}'.")
message(STATUS "[INFO] Build examples           -> '${FIFEGUI_EXAMPLES}'.")
message(STATUS "---------------------------------------------------------------")

#------------------------------------------------------------------------------
# Configure
#------------------------------------------------------------------------------

set(FIFECHAN_EXAMPLE_INSTALL_BASE "${CMAKE_INSTALL_DATADIR}/examples/fifechan")

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
    # Keep CRT selection consistent with BUILD_SHARED_LIBS and vcpkg triplet.
    if(BUILD_SHARED_LIBS)
      set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
    else()
      set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    endif()

    # Disable logo, enable exceptions, use full path in diagnostics,
    # be more standards compliant, use UTF-8 encoding, and enable Control Flow Guard.
    string(APPEND CMAKE_CXX_FLAGS " /nologo /EHsc /FC /permissive- /utf-8 /guard:cf")

    # Enable debug info in release builds for better crash dumps and profiling.
    string(APPEND CMAKE_CXX_FLAGS_RELEASE " /Zi")

    # ensure we use minimal "windows.h" lib without the crazy min max macros
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /D \"WIN32_LEAN_AND_MEAN\"")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /D \"NOMINMAX\"")
  endif()
else()
  # disable C++20 module support
  set(CMAKE_CXX_SCAN_FOR_MODULES 0)

  # Unix-like systems (Linux, macOS, etc.)
  if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    # Apple Clang defaults to libc++, but Linux Clang commonly targets libstdc++.
    # Forcing libc++ on Linux can fail when libc++ headers/runtime are unavailable.
    if(APPLE)
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
      set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++")
      set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -stdlib=libc++")
    else()
      message(STATUS "Using Clang ${CMAKE_CXX_COMPILER_VERSION} with default C++ standard library")
    endif()
  elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    message(STATUS "Using GCC ${CMAKE_CXX_COMPILER_VERSION} with libstdc++")
  endif()
endif()

#------------------------------------------------------------------------------
# Source Files for FifeGUI core library
#------------------------------------------------------------------------------

set(FIFECHAN_HEADER
    include/fifechan.hpp
)
set(FIFECHAN_HEADERS
    include/fifechan/cliprectangle.hpp
    include/fifechan/color.hpp
    include/fifechan/defaultfont.hpp
    include/fifechan/events/actionevent.hpp
    include/fifechan/events/containerevent.hpp
    include/fifechan/events/event.hpp
    include/fifechan/events/inputevent.hpp
    include/fifechan/events/keyevent.hpp
    include/fifechan/events/mouseevent.hpp
    include/fifechan/events/selectionevent.hpp
    include/fifechan/exception.hpp
    include/fifechan/focushandler.hpp
    include/fifechan/font.hpp
    include/fifechan/genericinput.hpp
    include/fifechan/graphics.hpp
    include/fifechan/groups.hpp
    include/fifechan/gui.hpp
    include/fifechan/image.hpp
    include/fifechan/imagefont.hpp
    include/fifechan/imageloader.hpp
    include/fifechan/input.hpp
    include/fifechan/key.hpp
    include/fifechan/keyinput.hpp
    include/fifechan/listeners/actionlistener.hpp
    include/fifechan/listeners/containerlistener.hpp
    include/fifechan/listeners/deathlistener.hpp
    include/fifechan/listeners/focuslistener.hpp
    include/fifechan/listeners/keylistener.hpp
    include/fifechan/listeners/mouselistener.hpp
    include/fifechan/listeners/selectionlistener.hpp
    include/fifechan/listeners/visibilityeventhandler.hpp
    include/fifechan/listeners/widgetlistener.hpp
    include/fifechan/listmodel.hpp
    include/fifechan/math.hpp
    include/fifechan/mouseinput.hpp
    include/fifechan/platform.hpp
    include/fifechan/point.hpp
    include/fifechan/rectangle.hpp
    include/fifechan/size.hpp
    include/fifechan/text.hpp
    include/fifechan/utf8stringeditor.hpp
    include/fifechan/util/utf8/utf8.hpp
    include/fifechan/util/utf8/utf8/checked.h
    include/fifechan/util/utf8/utf8/core.h
    include/fifechan/util/utf8/utf8/cpp11.h
    include/fifechan/util/utf8/utf8/cpp17.h
    include/fifechan/util/utf8/utf8/cpp20.h
    include/fifechan/util/utf8/utf8/unchecked.h
    include/fifechan/widget.hpp
    include/fifechan/widgets/adjustingcontainer.hpp
    include/fifechan/widgets/bargraph.hpp
    include/fifechan/widgets/button.hpp
    include/fifechan/widgets/checkbox.hpp
    include/fifechan/widgets/container.hpp
    include/fifechan/widgets/curvegraph.hpp
    include/fifechan/widgets/dropdown.hpp
    include/fifechan/widgets/flowcontainer.hpp
    include/fifechan/widgets/icon.hpp
    include/fifechan/widgets/iconprogressbar.hpp
    include/fifechan/widgets/imagebutton.hpp
    include/fifechan/widgets/imageprogressbar.hpp
    include/fifechan/widgets/label.hpp
    include/fifechan/widgets/linegraph.hpp
    include/fifechan/widgets/listbox.hpp
    include/fifechan/widgets/passwordfield.hpp
    include/fifechan/widgets/piegraph.hpp
    include/fifechan/widgets/pointgraph.hpp
    include/fifechan/widgets/radiobutton.hpp
    include/fifechan/widgets/scrollarea.hpp
    include/fifechan/widgets/slider.hpp
    include/fifechan/widgets/spacer.hpp
    include/fifechan/widgets/tab.hpp
    include/fifechan/widgets/tabbedarea.hpp
    include/fifechan/widgets/textbox.hpp
    include/fifechan/widgets/textfield.hpp
    include/fifechan/widgets/togglebutton.hpp
    include/fifechan/widgets/window.hpp
)
set(FIFECHAN_VERSION_HEADER
    "${CMAKE_CURRENT_BINARY_DIR}/include/fifechan/version.hpp"
)

set(FIFECHAN_SRC
    src/cliprectangle.cpp
    src/color.cpp
    src/defaultfont.cpp
    src/events/actionevent.cpp
    src/events/containerevent.cpp
    src/events/event.cpp
    src/events/inputevent.cpp
    src/events/keyevent.cpp
    src/events/mouseevent.cpp
    src/events/selectionevent.cpp
    src/fifechan.cpp
    src/focushandler.cpp
    src/font.cpp
    src/genericinput.cpp
    src/graphics.cpp
    src/gui.cpp
    src/image.cpp
    src/imagefont.cpp
    src/key.cpp
    src/keyinput.cpp
    src/mouseinput.cpp
    src/rectangle.cpp
    src/size.cpp
    src/text.cpp
    src/utf8stringeditor.cpp
    src/visibilityeventhandler.cpp
    src/widget.cpp
    src/widgets/adjustingcontainer.cpp
    src/widgets/bargraph.cpp
    src/widgets/button.cpp
    src/widgets/checkbox.cpp
    src/widgets/container.cpp
    src/widgets/curvegraph.cpp
    src/widgets/dropdown.cpp
    src/widgets/flowcontainer.cpp
    src/widgets/icon.cpp
    src/widgets/iconprogressbar.cpp
    src/widgets/imagebutton.cpp
    src/widgets/imageprogressbar.cpp
    src/widgets/label.cpp
    src/widgets/linegraph.cpp
    src/widgets/listbox.cpp
    src/widgets/passwordfield.cpp
    src/widgets/piegraph.cpp
    src/widgets/pointgraph.cpp
    src/widgets/radiobutton.cpp
    src/widgets/scrollarea.cpp
    src/widgets/slider.cpp
    src/widgets/spacer.cpp
    src/widgets/tab.cpp
    src/widgets/tabbedarea.cpp
    src/widgets/textbox.cpp
    src/widgets/textfield.cpp
    src/widgets/togglebutton.cpp
    src/widgets/window.cpp
)

#------------------------------------------------------------------------------
# FifeGUI core library
#------------------------------------------------------------------------------

add_library(${PROJECT_NAME} ${FIFECHAN_LIBRARY_BUILD_TYPE}
    ${FIFECHAN_SRC}
)

# Provide a namespace alias for the library (fifechan::fifechan)
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

# define public interface for the library
target_sources(${PROJECT_NAME}
    PUBLIC
    FILE_SET public_headers TYPE HEADERS
    BASE_DIRS include
    FILES
      ${FIFECHAN_HEADER}
      ${FIFECHAN_HEADERS}

    PUBLIC
    FILE_SET generated_headers TYPE HEADERS
    BASE_DIRS ${CMAKE_CURRENT_BINARY_DIR}/include
    FILES
      ${FIFECHAN_VERSION_HEADER}
)

target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include> # to include the generated "version.hpp"
    $<INSTALL_INTERFACE:include>
)

target_compile_definitions(${PROJECT_NAME} PUBLIC
  $<$<NOT:$<BOOL:${BUILD_SHARED_LIBS}>>:FIFECHAN_STATIC>
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

install(TARGETS ${PROJECT_NAME}
  EXPORT ${PROJECT_NAME}-targets

  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}

  FILE_SET public_headers
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}

  FILE_SET generated_headers
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}

  PERMISSIONS
    OWNER_READ OWNER_WRITE OWNER_EXECUTE
    GROUP_READ GROUP_EXECUTE
    WORLD_READ WORLD_EXECUTE
)

#------------------------------------------------------------------------------
# The FifeGUI OpenGL extension library
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
  set(FIFECHAN_OPENGL_HEADERS
    include/fifechan/backends/opengl/graphics.hpp
    include/fifechan/backends/opengl/image.hpp
    include/fifechan/backends/opengl/imageloader.hpp
    include/fifechan/backends/opengl/opengl.hpp
  )
  set(FIFECHAN_OPENGL_SRC
    src/backends/opengl/graphics.cpp
    src/backends/opengl/image.cpp
  )

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

  target_compile_definitions(${PROJECT_NAME}_opengl PUBLIC
    $<$<NOT:$<BOOL:${BUILD_SHARED_LIBS}>>:FIFECHAN_STATIC>
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
# The FifeGUI SDL extension library
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

if(ENABLE_SDL)

  # The FifeGUI SDL extension source
  set(FIFECHAN_SDL_HEADERS
    include/fifechan/backends/sdl2/graphics.hpp
    include/fifechan/backends/sdl2/image.hpp
    include/fifechan/backends/sdl2/imageloader.hpp
    include/fifechan/backends/sdl2/input.hpp
    include/fifechan/backends/sdl2/pixel.hpp
    include/fifechan/backends/sdl2/sdl.hpp
    include/fifechan/backends/sdl2/truetypefont.hpp
  )
  file(GLOB FIFECHAN_SDL_SRC
    src/backends/sdl2/graphics.cpp
    src/backends/sdl2/image.cpp
    src/backends/sdl2/imageloader.cpp
    src/backends/sdl2/input.cpp
    src/backends/sdl2/truetypefont.cpp
  )

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

  target_compile_definitions(${PROJECT_NAME}_sdl2 PUBLIC
    $<$<NOT:$<BOOL:${BUILD_SHARED_LIBS}>>:FIFECHAN_STATIC>
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

if(FIFEGUI_EXAMPLES)
  add_subdirectory(examples)
endif()

#------------------------------------------------------------------------------
# Tests
#------------------------------------------------------------------------------

if(FIFEGUI_TESTS)
  # enable unit tests
  enable_testing()
  add_subdirectory(tests/unit)

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
