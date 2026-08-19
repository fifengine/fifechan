message(STATUS "---------------------------------------------------------------")
message(STATUS "[ 🛠️ ] Building Integration Tests for SDL3")
message(STATUS "---------------------------------------------------------------")

find_package(SDL3_ttf CONFIG REQUIRED)

set(FIFEGUI_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include")

# For "version.hpp"
set(FIFEGUI_VERSION_FILE_DIR "${CMAKE_CURRENT_BINARY_DIR}/../../../include")

if(DEFINED ENV{VCPKG_ROOT})
  include("$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
endif()

#------------------------------------------------------------------------------
# sdlhelloworld
#------------------------------------------------------------------------------

add_executable(sdlhelloworld sdlhelloworld.cpp sdlhelloworld.hpp)

target_link_libraries(sdlhelloworld PRIVATE
  $<IF:$<TARGET_EXISTS:SDL3::SDL3>,SDL3::SDL3,SDL3::SDL3-static>
  $<IF:$<TARGET_EXISTS:SDL3_image::SDL3_image>,SDL3_image::SDL3_image,SDL3_image::SDL3_image-static>
  $<IF:$<TARGET_EXISTS:SDL3_ttf::SDL3_ttf>,SDL3_ttf::SDL3_ttf,SDL3_ttf::SDL3_ttf-static>
  fifechan::fifechan
  fifechan::fifechan_sdl3
)

set_target_properties(sdlhelloworld PROPERTIES
  CXX_STANDARD           20
  CXX_STANDARD_REQUIRED  ON
)

if(WIN32)
  set_target_properties(sdlhelloworld PROPERTIES
    WIN32_EXECUTABLE TRUE
  )
endif()

# This toggles the FIFEGUI_BUILD type (fifechan_EXPORTS, fifegui_EXPORTS)
if(BUILD_SHARED_LIBS)
  target_compile_definitions(sdlhelloworld PRIVATE "FIFEGUI_DLL_IMPORT") #"FIFEGUI_BUILD;FIFEGUI_EXTENSION_BUILD")
else()
  target_compile_definitions(sdlhelloworld INTERFACE "FIFEGUI_STATIC;FIFEGUI_EXT_STATIC")
endif()

show_build_target_properties(sdlhelloworld)

set(RESOURCES_FOLDER "${PROJECT_SOURCE_DIR}/tests/resources")
copy_resources_folder(sdlhelloworld "${RESOURCES_FOLDER}")

# Install resources: copy individual resource files directly into the install
# binary directory so they are located next to the installed executable.
file(GLOB SDL_RESOURCE_FILES "${RESOURCES_FOLDER}/*")
install(FILES ${SDL_RESOURCE_FILES} DESTINATION "${CMAKE_INSTALL_BINDIR}")

# Install VCPKG DLLs
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    file(GLOB VCPKG_DLLS "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/bin/*.dll")
    install(FILES ${VCPKG_DLLS} DESTINATION "${CMAKE_INSTALL_BINDIR}")
endif()

# Install VCPKG debug DLLs
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    file(GLOB VCPKG_DLLS "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/debug/bin/*.dll")
    install(FILES ${VCPKG_DLLS} DESTINATION "${CMAKE_INSTALL_BINDIR}")
endif()

# Install the .pdb file if it exists (for MSVC)
#install(FILES "$<TARGET_PDB_FILE:sdlhelloworld>" DESTINATION "${CMAKE_INSTALL_BINDIR}" OPTIONAL)

# Install the executables so the installed tree contains the binaries
# next to the installed resources.
install(TARGETS sdlhelloworld
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

#------------------------------------------------------------------------------
# sdlwidgets
#------------------------------------------------------------------------------

add_executable(sdlwidgets sdlwidgets.cpp sdlwidgets.hpp)

target_link_libraries(sdlwidgets PRIVATE
  $<IF:$<TARGET_EXISTS:SDL3::SDL3>,SDL3::SDL3,SDL3::SDL3-static>
  $<IF:$<TARGET_EXISTS:SDL3_image::SDL3_image>,SDL3_image::SDL3_image,SDL3_image::SDL3_image-static>
  $<IF:$<TARGET_EXISTS:SDL3_ttf::SDL3_ttf>,SDL3_ttf::SDL3_ttf,SDL3_ttf::SDL3_ttf-static>
  fifechan::fifechan
  fifechan::fifechan_sdl3
)

set_target_properties(sdlwidgets PROPERTIES
  CXX_STANDARD           20
  CXX_STANDARD_REQUIRED  ON
)

if(WIN32)
  set_target_properties(sdlwidgets PROPERTIES
    WIN32_EXECUTABLE TRUE
  )
endif()

# This toggles the FIFEGUI_BUILD type (fifechan_EXPORTS, fifegui_EXPORTS)
if(BUILD_SHARED_LIBS)
  target_compile_definitions(sdlwidgets PRIVATE "FIFEGUI_DLL_IMPORT")
else()
  target_compile_definitions(sdlwidgets INTERFACE "FIFEGUI_STATIC;FIFEGUI_EXT_STATIC")
endif()

show_build_target_properties(sdlwidgets)

copy_resources_folder(sdlwidgets "${RESOURCES_FOLDER}")

# Install the executables so the installed tree contains the binaries
# next to the installed resources.
install(TARGETS sdlwidgets
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

#------------------------------------------------------------------------------
# sdllabels
#------------------------------------------------------------------------------

add_executable(sdllabels sdllabels.cpp sdllabels.hpp)

target_link_libraries(sdllabels PRIVATE
  $<IF:$<TARGET_EXISTS:SDL3::SDL3>,SDL3::SDL3,SDL3::SDL3-static>
  $<IF:$<TARGET_EXISTS:SDL3_image::SDL3_image>,SDL3_image::SDL3_image,SDL3_image::SDL3_image-static>
  $<IF:$<TARGET_EXISTS:SDL3_ttf::SDL3_ttf>,SDL3_ttf::SDL3_ttf,SDL3_ttf::SDL3_ttf-static>
  fifechan::fifechan
  fifechan::fifechan_sdl3
)

set_target_properties(sdllabels PROPERTIES
  CXX_STANDARD           20
  CXX_STANDARD_REQUIRED  ON
)

if(WIN32)
  set_target_properties(sdllabels PROPERTIES
    WIN32_EXECUTABLE TRUE
  )
endif()

# This toggles the FIFEGUI_BUILD type (fifechan_EXPORTS, fifegui_EXPORTS)
if(BUILD_SHARED_LIBS)
  target_compile_definitions(sdllabels PRIVATE "FIFEGUI_DLL_IMPORT")
else()
  target_compile_definitions(sdllabels INTERFACE "FIFEGUI_STATIC;FIFEGUI_EXT_STATIC")
endif()

show_build_target_properties(sdllabels)

copy_resources_folder(sdllabels "${RESOURCES_FOLDER}")

# Install the executables so the installed tree contains the binaries
# next to the installed resources.
install(TARGETS sdllabels
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

#------------------------------------------------------------------------------
# sdldragdrop
#------------------------------------------------------------------------------

add_executable(sdldragdrop sdldragdrop.cpp)

target_link_libraries(sdldragdrop PRIVATE
  $<IF:$<TARGET_EXISTS:SDL3::SDL3>,SDL3::SDL3,SDL3::SDL3-static>
  $<IF:$<TARGET_EXISTS:SDL3_image::SDL3_image>,SDL3_image::SDL3_image,SDL3_image::SDL3_image-static>
  $<IF:$<TARGET_EXISTS:SDL3_ttf::SDL3_ttf>,SDL3_ttf::SDL3_ttf,SDL3_ttf::SDL3_ttf-static>
  fifechan::fifechan
  fifechan::fifechan_sdl3
)

set_target_properties(sdldragdrop PROPERTIES
  CXX_STANDARD           20
  CXX_STANDARD_REQUIRED  ON
)

if(WIN32)
  set_target_properties(sdldragdrop PROPERTIES
    WIN32_EXECUTABLE TRUE
  )
endif()

# This toggles the FIFEGUI_BUILD type (fifechan_EXPORTS, fifegui_EXPORTS)
if(BUILD_SHARED_LIBS)
  target_compile_definitions(sdldragdrop PRIVATE "FIFEGUI_DLL_IMPORT")
else()
  target_compile_definitions(sdldragdrop INTERFACE "FIFEGUI_STATIC;FIFEGUI_EXT_STATIC")
endif()

show_build_target_properties(sdldragdrop)

copy_resources_folder(sdldragdrop "${RESOURCES_FOLDER}")

# Install the executables so the installed tree contains the binaries
# next to the installed resources.
install(TARGETS sdldragdrop
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

#------------------------------------------------------------------------------
# sdlmdedit
#------------------------------------------------------------------------------

add_executable(sdlmdedit sdlmdedit.cpp)

target_link_libraries(sdlmdedit PRIVATE
  $<IF:$<TARGET_EXISTS:SDL3::SDL3>,SDL3::SDL3,SDL3::SDL3-static>
  $<IF:$<TARGET_EXISTS:SDL3_image::SDL3_image>,SDL3_image::SDL3_image,SDL3_image::SDL3_image-static>
  fifechan::fifechan
  fifechan::fifechan_sdl3
)

set_target_properties(sdlmdedit PROPERTIES
  CXX_STANDARD           20
  CXX_STANDARD_REQUIRED  ON
)

if(WIN32)
  set_target_properties(sdlmdedit PROPERTIES
    WIN32_EXECUTABLE TRUE
  )
endif()

# This toggles the FIFEGUI_BUILD type (fifechan_EXPORTS, fifegui_EXPORTS)
if(BUILD_SHARED_LIBS)
  target_compile_definitions(sdlmdedit PRIVATE "FIFEGUI_DLL_IMPORT")
else()
  target_compile_definitions(sdlmdedit INTERFACE "FIFEGUI_STATIC;FIFEGUI_EXT_STATIC")
endif()

show_build_target_properties(sdlmdedit)

copy_resources_folder(sdlmdedit "${RESOURCES_FOLDER}")

# Install the executables so the installed tree contains the binaries
# next to the installed resources.
install(TARGETS sdlmdedit
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

#------------------------------------------------------------------------------
# sdlspeechbubble
#------------------------------------------------------------------------------

add_executable(sdlspeechbubble sdlspeechbubble.cpp sdlspeechbubble.hpp)

target_link_libraries(sdlspeechbubble PRIVATE
  $<IF:$<TARGET_EXISTS:SDL3::SDL3>,SDL3::SDL3,SDL3::SDL3-static>
  $<IF:$<TARGET_EXISTS:SDL3_image::SDL3_image>,SDL3_image::SDL3_image,SDL3_image::SDL3_image-static>
  $<IF:$<TARGET_EXISTS:SDL3_ttf::SDL3_ttf>,SDL3_ttf::SDL3_ttf,SDL3_ttf::SDL3_ttf-static>
  fifechan::fifechan
  fifechan::fifechan_sdl3
)

set_target_properties(sdlspeechbubble PROPERTIES
  CXX_STANDARD           20
  CXX_STANDARD_REQUIRED  ON
)

if(WIN32)
  set_target_properties(sdlspeechbubble PROPERTIES
    WIN32_EXECUTABLE TRUE
  )
endif()

if(BUILD_SHARED_LIBS)
  target_compile_definitions(sdlspeechbubble PRIVATE "FIFEGUI_DLL_IMPORT")
else()
  target_compile_definitions(sdlspeechbubble INTERFACE "FIFEGUI_STATIC;FIFEGUI_EXT_STATIC")
endif()

show_build_target_properties(sdlspeechbubble)

copy_resources_folder(sdlspeechbubble "${RESOURCES_FOLDER}")

install(TARGETS sdlspeechbubble
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
