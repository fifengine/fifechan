# Changelog

All changes to the project will be documented in this file.

- The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).
- The date format is YYYY-MM-DD.
- The upcoming release version is named `vNext` and links to the changes between latest version tag and git HEAD.

## [vNext] - unreleased

## [0.2.0] - 2026-03-15

### Added

- added containerization for development by using devcontainer
- added continuous integration on Github Actions
- added automatic dependency updates on the Github Action workflows using dependabot
- added CMakePresets using composable config presets
  - with base presets in base.json
    - buildDirectory is ${sourceDir}/out/build/${presetName}
    - installDir is ${sourceDir}/out/install/${presetName}
  - platform specific presets in linux.json and windows.json
- added .clang-format and applied formatting
- added .clang-tidy and applied basic rule set
- added cpplint.sh, format.sh, tidy.sh to build-tools to reliably run C++ QA tools
- added CPPLINT.cfg files to the source tree for configuration and exlusion of files from cpplint runs
- added vcpkg for dependency management
- added SetupVcpkg for automatic installation of vcpkg packages during CMake configure step
- made vcpkg.json the single source for the version number and generate a version.hpp
- add show_build_target_properties() to CMakeLists
- add fifechan_install_example() for installing example files
- added CMake namespaced aliases for core library and extensions, e.g.
  - fifechan::fifechan, fifechan::fifechan_sdl, etc.
- reintegrated https://github.com/fifengine/fifechan-demos
  - the examples are now in the `tests/integration` folder, https://github.com/fifengine/fifechan/issues/37
  - converted build scripts from autotools to cmake, https://github.com/fifengine/fifechan-demos/issues/1
- added Catch2 as a testing framework for unit tests.
  - prepped a `tests/unit` folder for someone inclined to work on unit tests.
- added SDL2Graphics
- added support for target and package config (find_package), https://github.com/fifengine/fifechan/issues/19
- added pkg-config support to CMakeLists (for pkg-config), https://github.com/fifengine/fifechan/issues/19
- added handling of conditional dependencies in exported CMake configs
  - this enables upstream project to retire their finder scripts (FindFifechan.cmake)
- reintegrated all fifechan-demos into this repository (/examples) and updated
  them to work with the new CMake build system and SDL2
  - removed the old and deprecated autotools build system and all related files (configure.ac, Makefile)

### Changed

- all third-party library addressing backends moved into the folder "backends"
  - e.g. SDL backend lives now in "include/fifechan/backends/sdl" and "src/backends/sdl"
- moved each "unified header file" of a backend from the fifechan include folder ("include/fifechan")
  to the include folder of the backend itself
  - "include/fifechan/opengl.hpp" -> "include/fifechan/backends/opengl/opengl.hpp"
- modernize CMakeList syntax and formatting, e.g. switch from include_directories() to target_include_directories(), etc.
- modernised license headers and copyright notices using SPDX tags
- FCN_CORE_DECLSPEC -> FIFEGUI_API & FCN_EXTENSION_DECLSPEC -> FIFEGUI_EXT_API
- fix WIN32_LEAN_AND_MEAN redeclaration errors
- integrated "contributions" to allegro, opengl, sdl into the main folders
  - removed ENABLE_OPENGL_CONTRIB, ENABLE_SDL_CONTRIB, ENABLE_ALLEGRO_CONTRIB
  - opengl has now a dependency on oglft, freetype
  - sdl has now a dependency on sdl_ttf
- move includes/util/fcn_math.hpp to includes/math.hpp
- updated `tests/integration/sdlhelloworld` to work with SDL2
- in sdlpixel.hpp SDLAlpha32() renamed to SDLBlend(), SDLAlpha16() renamed to SDLBlendColor()
- renamed variable carea to clip_rect
- applied spellchecking
- renamed function parameter recursiv to recursion
- fixed all Doxygen warnings "found unknown command"

### Removed

- removed backends: allegro, cairo, irrlicht, glut, hge, openlayer
- removed usage of deprecated std::iterator
- remove usage of do-while loops
- removed all extern "c" helper functions for extension detection by autotools
- removed source_group() support for Visual Studio IDE

## [0.1.5] - 2019-01-11

- Added AT Key
- Added Auto layouting
- Fixed compile problems
- Fixed some more bugs

## [0.1.4] - 2017-02-07

- Added mouse wheel left and right events
- Fixed another possible segfault situation, issue #45

## [0.1.3] - 2016-11-05

- Requires CMake 2.6.0 to use Freetype find_package function.
  Freetype is required by oglft.
- Updated to SDL2
- Updated FPS demo for SDL2
- Bug fixes
- travis build fixes
- added Appveyor build configuration
- added Auto layouting
- fixes some possible segfaults
- Enhanced modal focus handling
- added a new logo

## [0.1.2] - 2014-08-10

- Added the possibility for horizontal scrolling.
- Added isSetVisible() to the Widget class. It's very similar to isVisible(),
  but ignores the parent.
- Added GuiDeathListener. That should prevent crashes if the widget is hidden
  and removed on the same pump.
- Added ${LIB_SUFFIX} for CMake lib path. Can be used via -DLIB_SUFFIX argument.
- Fixed a segfault that could happen if the Widgets are destroyed after the Gui.

## [0.1.1] - 2014-02-23

- Correct CMake SDL_ttf module name by Christopher Meng
- Setting the selected property of dropdowns to a value < 0 now
  will make them deselect the current item.
- Fixed extensions not being built with the correct export definition.
  This will fix undefined references when linking
  with the created shared libraries.
- Changed cmake options to enable contrib extensions for sdl and opengl
  to be OFF by default.
- OGLFTFone is now compiled into fifechan. It can be enabled or disabled
  by setting the cmake option ENABLE_OPENGL_CONTRIB to ON or OFF.
  It is enabled by default.
- Fixed SDLTrueTypeFont not included by CMake. It can be disabled by setting
  ENABLE_SDL_CONTRIB to OFF, which defaults to ON.
- Add SDLIMAGE_INCLUDE_DIR to CMake by Nicolás Alvarez

## [0.1.0] - 2012-12-05

There are already some important bug fixes and new features such as:

- A Widget is now able to detect that the mouse enters it when a Widget above
    it gets hidden and the mouse cursor was over it.
- Text objects do not try to access rows if they are empty, which caused
    crashes before.
- Widgets now distribute events if their ancestors where shown/hidden.
- UTF8 support for TextBox and TextField widgets.
- Work is done in a separate branch to support auto-layouting in fifechan.
  Needs to be improved and tested.
- New widgets
  - Circular container, which implements a container that arranges its children
    in a circular fashion. (only available in the auto-layouting branch)
  - IconProgressBar, which implements a progressbar that uses images to show
    progress.
  - PasswordField, which implements password input.

<!-- Section for Reference Links -->

[Unreleased]: https://github.com/fifengine/fifechan/compare/0.2.0...HEAD
[0.2.0]: https://github.com/fifengine/fifechan/compare/0.1.5...0.2.0
[0.1.5]: https://github.com/fifengine/fifechan/compare/0.1.4...0.1.5
[0.1.4]: https://github.com/fifengine/fifechan/compare/0.1.3...0.1.4
[0.1.3]: https://github.com/fifengine/fifechan/compare/0.1.2...0.1.3
[0.1.2]: https://github.com/fifengine/fifechan/compare/0.1.1...0.1.2
[0.1.1]: https://github.com/fifengine/fifechan/compare/0.1.0...0.1.1
[0.1.0]: https://github.com/fifengine/fifechan/releases/tag/0.1.0
