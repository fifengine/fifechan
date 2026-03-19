// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_BACKENDS_SDL_SDL_HPP_
#define INCLUDE_FIFECHAN_BACKENDS_SDL_SDL_HPP_

/**
 * Unified header for the SDL backend.
 */
#include <SDL2/SDL.h>

#include <fifechan/backends/sdl2/graphics.hpp>
#include <fifechan/backends/sdl2/image.hpp>
#include <fifechan/backends/sdl2/imageloader.hpp>
#include <fifechan/backends/sdl2/input.hpp>
#include <fifechan/backends/sdl2/truetypefont.hpp>
#include <fifechan/platform.hpp>

#include <memory>

namespace fcn::sdl2
{
    /** Backend alias for the SDL2 graphics implementation. */
    using Graphics = fcn::sdl2::Graphics;

    /** Backend alias for the SDL image implementation. */
    using Image = fcn::sdl2::Image;

    /** Backend alias for the SDL image loader. */
    using ImageLoader = fcn::sdl2::ImageLoader;

    /** Backend alias for the SDL input implementation. */
    using Input = fcn::sdl2::Input;

    /** Backend alias for the SDL TrueType font wrapper. */
    using TrueTypeFont = fcn::sdl2::TrueTypeFont;

    /** Create a shared_ptr that will destroy the SDL_Window when released. */
    inline std::shared_ptr<SDL_Window> makeSDLSharedPtr(SDL_Window* window)
    {
        return std::shared_ptr<SDL_Window>{window, [](SDL_Window* value) {
                                               if (value != nullptr) {
                                                   SDL_DestroyWindow(value);
                                               }
                                           }};
    }

    /** Create a shared_ptr that will destroy the SDL_Renderer when released. */
    inline std::shared_ptr<SDL_Renderer> makeSDLSharedPtr(SDL_Renderer* renderer)
    {
        return std::shared_ptr<SDL_Renderer>{renderer, [](SDL_Renderer* value) {
                                                 if (value != nullptr) {
                                                     SDL_DestroyRenderer(value);
                                                 }
                                             }};
    }

    /** Create a shared_ptr that will free the SDL_Surface when released. */
    inline std::shared_ptr<SDL_Surface> makeSDLSharedPtr(SDL_Surface* surface)
    {
        return std::shared_ptr<SDL_Surface>{surface, [](SDL_Surface* value) {
                                                if (value != nullptr) {
                                                    SDL_FreeSurface(value);
                                                }
                                            }};
    }

    /** Create a shared_ptr that will destroy the SDL_Texture when released. */
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
