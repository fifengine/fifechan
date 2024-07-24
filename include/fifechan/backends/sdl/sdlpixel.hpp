// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_BACKENDS_SDL_SDLPIXEL_HPP_
#define INCLUDE_FIFECHAN_BACKENDS_SDL_SDLPIXEL_HPP_

#include <SDL2/SDL.h>

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
    inline Color const SDLgetPixel(SDL_Surface* surface, int x, int y)
    {
        int bpp = surface->format->BytesPerPixel;

        SDL_LockSurface(surface);

        Uint8* p = (Uint8*)surface->pixels + y * surface->pitch + x * bpp;

        unsigned int color = 0;

        switch (bpp) {
        case 1:
            color = *p;
            break;

        case 2:
            color = *(Uint16*)p;
            break;

        case 3:
            if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
                color = p[0] << 16 | p[1] << 8 | p[2];
            } else {
                color = p[0] | p[1] << 8 | p[2] << 16;
            }
            break;

        case 4:
            color = *(Uint32*)p;
            break;
        }

        unsigned char r, g, b, a;

        SDL_GetRGBA(color, surface->format, &r, &g, &b, &a);
        SDL_UnlockSurface(surface);

        return Color(r, g, b, a);
    }

    /**
     * Puts a pixel on an SDL_Surface.
     *
     * @param x the x coordinate on the surface.
     * @param y the y coordinate on the surface.
     * @param color the color the pixel should be in.
     */
    inline void SDLputPixel(SDL_Surface* surface, int x, int y, Color const & color)
    {
        int bpp = surface->format->BytesPerPixel;

        SDL_LockSurface(surface);

        Uint8* p = (Uint8*)surface->pixels + y * surface->pitch + x * bpp;

        Uint32 pixel = SDL_MapRGB(surface->format, color.r, color.g, color.b);

        switch (bpp) {
        case 1:
            *p = pixel;
            break;

        case 2:
            *(Uint16*)p = pixel;
            break;

        case 3:
            if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
                p[0] = (pixel >> 16) & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = pixel & 0xff;
            } else {
                p[0] = pixel & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = (pixel >> 16) & 0xff;
            }
            break;

        case 4:
            *(Uint32*)p = pixel;
            break;
        }

        SDL_UnlockSurface(surface);
    }

    /**
     * Blends two color components together.
     *
     * @param src the source color.
     * @param dst the destination color.
     * @param a alpha.
     */
    template <typename T>
    inline T SDLBlend(T source, T dest, unsigned char alpha)
    {
        return dest + ((source - dest) * alpha >> 8);
    }

    /**
     * Blends two colors together.
     *
     * @param src the source color.
     * @param dst the destination color.
     * @param a alpha.
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
     * Puts a pixel on an SDL_Surface with alpha
     *
     * @param x the x coordinate on the surface.
     * @param y the y coordinate on the surface.
     * @param color the color the pixel should be in.
     */
    inline void SDLputPixelAlpha(SDL_Surface* surface, int x, int y, Color const & color)
    {
        // avoids overhead and truncation artifacts
        if (color.a == 255) {
            SDLputPixel(surface, x, y, color);
            return;
        }

        int bpp = surface->format->BytesPerPixel;

        SDL_LockSurface(surface);

        Uint8* p = (Uint8*)surface->pixels + y * surface->pitch + x * bpp;

        switch (bpp) {
        case 1: {
            Uint32 pixel = SDL_MapRGB(surface->format, color.r, color.g, color.b);

            SDL_Color* colors      = surface->format->palette->colors;
            SDL_Color& sourceColor = colors[pixel];
            SDL_Color& destColor   = colors[*p];

            *p = SDL_MapRGB(
                surface->format,
                SDLBlend(sourceColor.r, destColor.r, color.a),
                SDLBlend(sourceColor.g, destColor.g, color.a),
                SDLBlend(sourceColor.b, destColor.b, color.a));
            break;
        }
        case 2: {
            Uint32 pixel = SDL_MapRGB(surface->format, color.r, color.g, color.b);
            *(Uint16*)p  = SDLBlendColor<Uint16>(pixel, *(Uint16*)p, color.a, surface->format);
            break;
        }
        case 3: {
            if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
                // watch color order rgb
                p[0] = SDLBlend<Uint8>(color.r, p[0], color.a);
                p[1] = SDLBlend<Uint8>(color.g, p[1], color.a);
                p[2] = SDLBlend<Uint8>(color.b, p[2], color.a);
            } else {
                // watch color order bgr
                p[0] = SDLBlend<Uint8>(color.b, p[0], color.a);
                p[1] = SDLBlend<Uint8>(color.g, p[1], color.a);
                p[2] = SDLBlend<Uint8>(color.r, p[2], color.a);
            }
            break;
        }
        case 4: {
            Uint32 pixel = SDL_MapRGB(surface->format, color.r, color.g, color.b);
            *(Uint32*)p  = SDLBlendColor<Uint32>(pixel, *(Uint32*)p, color.a, surface->format);
            break;
        }

            SDL_UnlockSurface(surface);
        }
    }
} // namespace fcn

#endif // INCLUDE_FIFECHAN_BACKENDS_SDL_SDLPIXEL_HPP_
