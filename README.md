![FIFEGUI LOGO](https://raw.githubusercontent.com/fifengine/fifechan/master/docs/logo/FIFEgui_small_c3.png)

## FifeGUI

| [Website](http://fifengine.net/) | [![#fife on Freenode](https://img.shields.io/badge/freenode-%23fife-green.svg)](https://webchat.freenode.net/?channels=fife) | [Changelog](https://github.com/fifengine/fifechan/blob/master/CHANGELOG.md) | [Releases](https://github.com/fifengine/fifechan/releases) | [Source Code (zip)](https://github.com/fifengine/fifechan/archive/master.zip) |
|:--------:|:--------:|:--------:|:--------:|:--------:|

| Continuous Integration | Linux |   Mac    | Windows |
|:----------------------:|:-----:|:--------:|:-------:|
| **Build Status** | [![Build on Linux](https://github.com/fifengine/fifechan/actions/workflows/build-linux.yml/badge.svg)](https://github.com/fifengine/fifechan/actions/workflows/build-linux.yml) | Not Build | [![Build on Windows](https://github.com/fifengine/fifechan/actions/workflows/build-windows.yml/badge.svg)](https://github.com/fifengine/fifechan/actions/workflows/build-windows.yml) |

## Introduction to FifeGUI

FifeGUI is a lightweight, cross-platform C++ GUI library designed for games.

It offers a simple but powerful set of customizable widgets, allowing users to
create a wide range of widget types.

It supports rendering in SDL or OpenGL out of the box and it can be adapted to
use any rendering engine the user requires.

Events are pushed to FifeGUI, letting users choose their preferred input library
or use its built-in SDL input handling.

The main goal of FifeGUI is to remain lightweight, extendable,
and powerful enough to be used in any type of game.

## Downloads

#### Latest Releases

You find the latest releases on [Github Releases](https://github.com/fifengine/fifechan/releases).

#### Development Releases

We also provide releases for the latest successful build on Github Actions CI.

#### Availability in Package Repositories

FifeGUI is also already available from the following package repositories:

[![Packaging status](https://repology.org/badge/vertical-allrepos/fifechan.svg)](https://repology.org/metapackage/fifechan)

## License

FifeGUI is dual licensed under the [LGPL-2.1 License](/docs/license/LGPL-2.1-License.md) and [BSD License](/docs/license/BSD-License.md).

## Dev Notes

## Build Dependencies

You need the following libraries installed:

For OpenGL support:
 - OpenGL

For [SDL](https://libsdl.org) support:
 - SDL2
 - SDL2_image
 - SDL2_ttf
 - SDL2_mixer (optional) (FPS demo)
