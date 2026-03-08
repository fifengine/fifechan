// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_BACKENDS_SDL_SDL_HPP_
#define INCLUDE_FIFECHAN_BACKENDS_SDL_SDL_HPP_

/**
 * Unified header for the SDL backend.
 */
#include <SDL2/SDL.h>

#include <fifechan/backends/sdl2/sdl2graphics.hpp>
#include <fifechan/backends/sdl2/sdlimage.hpp>
#include <fifechan/backends/sdl2/sdlimageloader.hpp>
#include <fifechan/backends/sdl2/sdlinput.hpp>
#include <fifechan/backends/sdl2/sdltruetypefont.hpp>
#include <fifechan/platform.hpp>

#include <memory>

namespace fcn::sdl2
{
    using Graphics     = fcn::SDL2Graphics;
    using Image        = fcn::SDLImage;
    using ImageLoader  = fcn::SDLImageLoader;
    using Input        = fcn::SDLInput;
    using TrueTypeFont = fcn::SDLTrueTypeFont;

    inline std::shared_ptr<SDL_Window> makeSDLSharedPtr(SDL_Window* window)
    {
        return std::shared_ptr<SDL_Window>{window, [](SDL_Window* value) {
                                               if (value != nullptr) {
                                                   SDL_DestroyWindow(value);
                                               }
                                           }};
    }

    inline std::shared_ptr<SDL_Renderer> makeSDLSharedPtr(SDL_Renderer* renderer)
    {
        return std::shared_ptr<SDL_Renderer>{renderer, [](SDL_Renderer* value) {
                                                 if (value != nullptr) {
                                                     SDL_DestroyRenderer(value);
                                                 }
                                             }};
    }

    inline std::shared_ptr<SDL_Surface> makeSDLSharedPtr(SDL_Surface* surface)
    {
        return std::shared_ptr<SDL_Surface>{surface, [](SDL_Surface* value) {
                                                if (value != nullptr) {
                                                    SDL_FreeSurface(value);
                                                }
                                            }};
    }

    inline std::shared_ptr<SDL_Texture> makeSDLSharedPtr(SDL_Texture* texture)
    {
        return std::shared_ptr<SDL_Texture>{texture, [](SDL_Texture* value) {
                                                if (value != nullptr) {
                                                    SDL_DestroyTexture(value);
                                                }
                                            }};
    }
} // namespace fcn::sdl2

#endif // INCLUDE_FIFECHAN_BACKENDS_SDL_SDL_HPP_
