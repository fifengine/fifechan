message(STATUS "---------------------------------------------------------------")
message(STATUS "[ 🛠️ ] Building Integration Tests for SDL2")
message(STATUS "---------------------------------------------------------------")

# Find SDL2_ttf
find_package(SDL2_ttf REQUIRED)

set(FIFEGUI_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include")
# Build folder of the generated files from fifechan, e.g. "version.hpp"
set(FIFEGUI_VERSION_FILE_DIR "${CMAKE_CURRENT_BINARY_DIR}/../../../include")

# Include the VCPKG toolchain file if it is defined
if(DEFINED ENV{VCPKG_ROOT})
  include("$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
endif()

#------------------------------------------------------------------------------
# sdlhelloworld
#------------------------------------------------------------------------------

add_executable(sdlhelloworld sdlhelloworld.cpp sdlhelloworld.hpp)

target_include_directories(sdlhelloworld PRIVATE
  ${SDL2_INCLUDE_DIRS}
  ${FIFEGUI_INCLUDE_DIR}
  ${FIFEGUI_VERSION_FILE_DIR}
  ${PROJECT_SOURCE_DIR}/include/fifechan/backends/sdl2
)

target_link_libraries(sdlhelloworld PRIVATE
  # SDL2
  $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
  $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
  # SDL2_image
  $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>
  # SDL2_ttf
  $<IF:$<TARGET_EXISTS:SDL2_ttf::SDL2_ttf>,SDL2_ttf::SDL2_ttf,SDL2_ttf::SDL2_ttf-static>
  # FifeGUI
  fifechan::fifechan
  # FifeGUI SDL extension
  fifechan::fifechan_sdl2
)

set_target_properties(sdlhelloworld PROPERTIES
  CXX_STANDARD           20
  CXX_STANDARD_REQUIRED  ON
)

# This toggles the FIFEGUI_BUILD type (fifechan_EXPORTS, fifegui_EXPORTS)
if(BUILD_SHARED_LIBS)
  target_compile_definitions(sdlhelloworld PRIVATE "FIFEGUI_DLL_IMPORT") #"FIFEGUI_BUILD;FIFEGUI_EXTENSION_BUILD")
else()
  target_compile_definitions(sdlhelloworld INTERFACE "FIFEGUI_STATIC;FIFEGUI_EXT_STATIC")
endif()

show_build_target_properties(sdlhelloworld)

# Copy resources
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

#------------------------------------------------------------------------------
# sdlwidgets
#------------------------------------------------------------------------------

add_executable(sdlwidgets sdlwidgets.cpp sdlwidgets.hpp)

# target_include_directories(sdlwidgets PRIVATE
#   ${SDL2_INCLUDE_DIR}
#   ${FIFEGUI_INCLUDE_DIR}
# )
target_include_directories(sdlwidgets PRIVATE
  ${SDL2_INCLUDE_DIR}
  ${FIFEGUI_INCLUDE_DIR}
  ${PROJECT_SOURCE_DIR}/include/fifechan/backends/sdl2
)

target_link_libraries(sdlwidgets PRIVATE
  # SDL2
  $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
  $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
  # SDL2_image
  $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>
  # SDL2_ttf
  $<IF:$<TARGET_EXISTS:SDL2_ttf::SDL2_ttf>,SDL2_ttf::SDL2_ttf,SDL2_ttf::SDL2_ttf-static>
  # FifeGUI
  fifechan::fifechan
  # FifeGUI SDL extension
  fifechan::fifechan_sdl2
)

set_target_properties(sdlwidgets PROPERTIES
  CXX_STANDARD           20
  CXX_STANDARD_REQUIRED  ON
)

# This toggles the FIFEGUI_BUILD type (fifechan_EXPORTS, fifegui_EXPORTS)
if(BUILD_SHARED_LIBS)
  target_compile_definitions(sdlhelloworld PRIVATE "FIFEGUI_DLL_IMPORT") #"FIFEGUI_BUILD;FIFEGUI_EXTENSION_BUILD")
else()
  target_compile_definitions(sdlhelloworld INTERFACE "FIFEGUI_STATIC;FIFEGUI_EXT_STATIC")
endif()

show_build_target_properties(sdlwidgets)

# Copy resources
copy_resources_folder(sdlwidgets "${RESOURCES_FOLDER}")

# Install the executables so the installed tree contains the binaries
# next to the installed resources.
install(TARGETS sdlhelloworld sdlwidgets
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
