// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_BACKENDS_SDL_SDLPIXEL_HPP_
#define INCLUDE_FIFECHAN_BACKENDS_SDL_SDLPIXEL_HPP_

#include <SDL2/SDL.h>

#include <cstddef>
#include <cstring>
#include <span>

#include "fifechan/color.hpp"

namespace fcn
{

    /**
     * Checks a pixels color of an SDL_Surface.
     *
     * @param surface an SDL_Surface where to check for a pixel color.
     * @param x the x coordinate on the surface.
     * @param y the y coordinate on the surface.
     * @return a color of a pixel.
     */
    inline Color SDLgetPixel(SDL_Surface* surface, int x, int y)
    {
        int const bpp = surface->format->BytesPerPixel;

        SDL_LockSurface(surface);

        std::ptrdiff_t const offset = (static_cast<std::ptrdiff_t>(y) * static_cast<std::ptrdiff_t>(surface->pitch)) +
                                      (static_cast<std::ptrdiff_t>(x) * static_cast<std::ptrdiff_t>(bpp));

        std::span<Uint8> pixels(
            reinterpret_cast<Uint8*>(surface->pixels),
            static_cast<size_t>(surface->h) * static_cast<size_t>(surface->pitch));
        size_t const idx = static_cast<size_t>(offset);

        unsigned int color = 0U;

        switch (bpp) {
        case 1:
            color = pixels[idx];
            break;

        case 2: {
            Uint16 tmp = 0;
            std::memcpy(&tmp, &pixels[idx], sizeof(Uint16));
            color = static_cast<unsigned int>(tmp);
            break;
        }

        case 3:
            if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
                color = (static_cast<unsigned int>(pixels[idx]) << 16) |
                        (static_cast<unsigned int>(pixels[idx + 1]) << 8) | static_cast<unsigned int>(pixels[idx + 2]);
            } else {
                color = static_cast<unsigned int>(pixels[idx]) | (static_cast<unsigned int>(pixels[idx + 1]) << 8) |
                        (static_cast<unsigned int>(pixels[idx + 2]) << 16);
            }
            break;

        case 4: {
            Uint32 tmp = 0;
            std::memcpy(&tmp, &pixels[idx], sizeof(Uint32));
            color = static_cast<unsigned int>(tmp);
            break;
        }
        }

        unsigned char r = 0;
        unsigned char g = 0;
        unsigned char b = 0;
        unsigned char a = 0;

        SDL_GetRGBA(color, surface->format, &r, &g, &b, &a);
        SDL_UnlockSurface(surface);

        return {r, g, b, a};
    }

    /**
     * Puts a pixel on an SDL_Surface.
     *
     * @param surface The SDL_Surface to write the pixel to.
     * @param x the x coordinate on the surface.
     * @param y the y coordinate on the surface.
     * @param color the color the pixel should be in.
     */
    inline void SDLputPixel(SDL_Surface* surface, int x, int y, Color const & color)
    {
        int const bpp = surface->format->BytesPerPixel;

        SDL_LockSurface(surface);

        std::ptrdiff_t const offset = (static_cast<std::ptrdiff_t>(y) * static_cast<std::ptrdiff_t>(surface->pitch)) +
                                      (static_cast<std::ptrdiff_t>(x) * static_cast<std::ptrdiff_t>(bpp));

        std::span<Uint8> pixels(
            reinterpret_cast<Uint8*>(surface->pixels),
            static_cast<size_t>(surface->h) * static_cast<size_t>(surface->pitch));
        size_t const idx = static_cast<size_t>(offset);

        Uint32 const pixel = SDL_MapRGB(surface->format, color.r, color.g, color.b);

        switch (bpp) {
        case 1:
            pixels[idx] = static_cast<Uint8>(pixel);
            break;

        case 2: {
            Uint16 tmp = static_cast<Uint16>(pixel);
            std::memcpy(&pixels[idx], &tmp, sizeof(Uint16));
            break;
        }

        case 3:
            if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
                pixels[idx]     = static_cast<Uint8>((pixel >> 16) & 0xff);
                pixels[idx + 1] = static_cast<Uint8>((pixel >> 8) & 0xff);
                pixels[idx + 2] = static_cast<Uint8>(pixel & 0xff);
            } else {
                pixels[idx]     = static_cast<Uint8>(pixel & 0xff);
                pixels[idx + 1] = static_cast<Uint8>((pixel >> 8) & 0xff);
                pixels[idx + 2] = static_cast<Uint8>((pixel >> 16) & 0xff);
            }
            break;

        case 4: {
            Uint32 tmp = pixel;
            std::memcpy(&pixels[idx], &tmp, sizeof(Uint32));
            break;
        }
        }

        SDL_UnlockSurface(surface);
    }

    /**
     * Blends two color components together.
     *
     * @param source the source color component.
     * @param dest the destination color component.
     * @param alpha alpha value (0-255) used for blending.
     */
    template <typename T>
    inline T SDLBlend(T source, T dest, unsigned char alpha)
    {
        return dest + ((source - dest) * alpha >> 8);
    }

    /**
     * Blends two packed color values together using the supplied pixel format.
     *
     * @param src the source packed color value.
     * @param dst the destination packed color value.
     * @param alpha alpha value (0-255) used for blending.
     * @param f pixel format describing masks/shifts for color components.
     */
    template <typename T>
    inline T SDLBlendColor(T src, T dst, unsigned char alpha, SDL_PixelFormat const * f)
    {
        return (SDLBlend(src & f->Rmask, dst & f->Rmask, alpha) & f->Rmask) |
               (SDLBlend(src & f->Gmask, dst & f->Gmask, alpha) & f->Gmask) |
               (SDLBlend(src & f->Bmask, dst & f->Bmask, alpha) & f->Bmask);
    }

    /*
    typedef struct{
        SDL_Palette *palette;
        Uint8  BitsPerPixel;
        Uint8  BytesPerPixel;
        Uint32 Rmask, Gmask, Bmask, Amask;
        Uint8  Rshift, Gshift, Bshift, Ashift;
        Uint8  Rloss, Gloss, Bloss, Aloss;
        Uint32 colorkey;
        Uint8  alpha;
    } SDL_PixelFormat;
    */

    /**
     * Puts a pixel on an SDL_Surface with alpha blending.
     *
     * @param surface The SDL_Surface to write the pixel to.
     * @param x the x coordinate on the surface.
     * @param y the y coordinate on the surface.
     * @param color the color the pixel should be in (including alpha).
     */
    inline void SDLputPixelAlpha(SDL_Surface* surface, int x, int y, Color const & color)
    {
        // avoids overhead and truncation artifacts
        if (color.a == 255) {
            SDLputPixel(surface, x, y, color);
            return;
        }

        int const bpp = surface->format->BytesPerPixel;

        // avoids overhead and truncation artifacts
        if (color.a == 255) {
            SDLputPixel(surface, x, y, color);
            return;
        }

        SDL_LockSurface(surface);

        std::ptrdiff_t const offset = (static_cast<std::ptrdiff_t>(y) * static_cast<std::ptrdiff_t>(surface->pitch)) +
                                      (static_cast<std::ptrdiff_t>(x) * static_cast<std::ptrdiff_t>(bpp));

        std::span<Uint8> pixels(
            reinterpret_cast<Uint8*>(surface->pixels),
            static_cast<size_t>(surface->h) * static_cast<size_t>(surface->pitch));
        size_t const idx = static_cast<size_t>(offset);

        switch (bpp) {
        case 1: {
            Uint32 const pixel = SDL_MapRGB(surface->format, color.r, color.g, color.b);

            SDL_Color* colors             = surface->format->palette->colors;
            SDL_Color const & sourceColor = colors[pixel];
            SDL_Color const & destColor   = colors[pixels[idx]];

            pixels[idx] = static_cast<Uint8>(SDL_MapRGB(
                surface->format,
                SDLBlend(sourceColor.r, destColor.r, color.a),
                SDLBlend(sourceColor.g, destColor.g, color.a),
                SDLBlend(sourceColor.b, destColor.b, color.a)));
            break;
        }
        case 2: {
            Uint32 const pixel = SDL_MapRGB(surface->format, color.r, color.g, color.b);
            Uint16 dest        = 0;
            std::memcpy(&dest, &pixels[idx], sizeof(Uint16));
            Uint16 result = SDLBlendColor<Uint16>(pixel, dest, color.a, surface->format);
            std::memcpy(&pixels[idx], &result, sizeof(Uint16));
            break;
        }
        case 3: {
            if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
                // watch color order rgb
                pixels[idx + 0] = SDLBlend<Uint8>(color.r, pixels[idx + 0], color.a);
                pixels[idx + 1] = SDLBlend<Uint8>(color.g, pixels[idx + 1], color.a);
                pixels[idx + 2] = SDLBlend<Uint8>(color.b, pixels[idx + 2], color.a);
            } else {
                // watch color order bgr
                pixels[idx + 0] = SDLBlend<Uint8>(color.b, pixels[idx + 0], color.a);
                pixels[idx + 1] = SDLBlend<Uint8>(color.g, pixels[idx + 1], color.a);
                pixels[idx + 2] = SDLBlend<Uint8>(color.r, pixels[idx + 2], color.a);
            }
            break;
        }
        case 4: {
            Uint32 const pixel = SDL_MapRGB(surface->format, color.r, color.g, color.b);
            Uint32 dest        = 0;
            std::memcpy(&dest, &pixels[idx], sizeof(Uint32));
            Uint32 result = SDLBlendColor<Uint32>(pixel, dest, color.a, surface->format);
            std::memcpy(&pixels[idx], &result, sizeof(Uint32));
            break;
        }
        }

        SDL_UnlockSurface(surface);
    }
} // namespace fcn

namespace fcn::sdl2
{
    using fcn::SDLBlend;
    using fcn::SDLBlendColor;
    using fcn::SDLgetPixel;
    using fcn::SDLputPixel;
    using fcn::SDLputPixelAlpha;
} // namespace fcn::sdl2

#endif // INCLUDE_FIFECHAN_BACKENDS_SDL_SDLPIXEL_HPP_
