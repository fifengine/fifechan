# ChangeLog

## [Unreleased]

## [0.1.3] - 2016-11-05

- Requires CMake 2.6.0 to use Freetype find_package function. Freetype is
  required by oglft.
- Updated so use SDL2
- Updated fps demo for SDL2
- Bug fixes
- travis build fixes
- added Auto layouting
- fixes some possible segfaults
- Enhanced modal focus handling
- added an new logo

## [0.1.2] - 2014-08-10

- Added the possibility for horizontal scrolling.
- Added isSetVisible() to the Widget class. It's very similar to isVisible()
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

[Unreleased]: https://github.com/fifengine/fifechan/compare/0.1.2...HEAD
[0.1.2]: https://github.com/fifengine/fifechan/compare/0.1.1...0.1.2
[0.1.1]: https://github.com/fifengine/fifechan/compare/0.1.0...0.1.1
[0.1.0]: https://github.com/jakoch/phantomjs-installer/releases/tag/0.1.0
