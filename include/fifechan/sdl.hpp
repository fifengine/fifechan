// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors
#ifndef FCN_SDL_HPP
#define FCN_SDL_HPP

#include <fifechan/sdl/sdlgraphics.hpp>
#include <fifechan/sdl/sdlimage.hpp>
#include <fifechan/sdl/sdlimageloader.hpp>
#include <fifechan/sdl/sdlinput.hpp>

#include "platform.hpp"

extern "C" {
/**
 * Exists to be able to check for Guichan SDL with autotools.
 */
FCN_EXTENSION_DECLSPEC extern void fcnSDL();
}

#endif // end FCN_SDL_HPP
