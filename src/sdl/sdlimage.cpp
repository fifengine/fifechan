// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#include "fifechan/sdl/sdlimage.hpp"

#include "fifechan/exception.hpp"
#include "fifechan/sdl/sdlimageloader.hpp"
#include "fifechan/sdl/sdlpixel.hpp"

namespace fcn
{
    SDLImage::SDLImage(SDL_Surface* surface, bool autoFree)
    {
        mAutoFree = autoFree;
        mSurface  = surface;
    }

    SDLImage::~SDLImage()
    {
        if (mAutoFree) {
            free();
        }
    }

    SDL_Surface* SDLImage::getSurface() const
    {
        return mSurface;
    }

    int SDLImage::getWidth() const
    {
        if (mSurface == NULL) {
            fcn::throwException(
                ("Trying to get the width of a non loaded image."),
                static_cast<char const *>(__FUNCTION__),
                __FILE__,
                __LINE__);
        }

        return mSurface->w;
    }

    int SDLImage::getHeight() const
    {
        if (mSurface == NULL) {
            fcn::throwException(
                ("Trying to get the height of a non loaded image."),
                static_cast<char const *>(__FUNCTION__),
                __FILE__,
                __LINE__);
        }

        return mSurface->h;
    }

    Color SDLImage::getPixel(int x, int y)
    {
        if (mSurface == NULL) {
            fcn::throwException(
                ("Trying to get a pixel from a non loaded image."),
                static_cast<char const *>(__FUNCTION__),
                __FILE__,
                __LINE__);
        }

        return SDLgetPixel(mSurface, x, y);
    }

    void SDLImage::putPixel(int x, int y, Color const & color)
    {
        if (mSurface == NULL) {
            fcn::throwException(
                ("Trying to put a pixel in a non loaded image."),
                static_cast<char const *>(__FUNCTION__),
                __FILE__,
                __LINE__);
        }

        SDLputPixel(mSurface, x, y, color);
    }

    void SDLImage::convertToDisplayFormat()
    {
        if (mSurface == NULL) {
            fcn::throwException(
                ("Trying to convert a non loaded image to display format."),
                static_cast<char const *>(__FUNCTION__),
                __FILE__,
                __LINE__);
        }

        SDLImageLoader* loader         = static_cast<SDLImageLoader*>(mImageLoader);
        SDL_PixelFormat const & format = loader->getSDLPixelFormat();

        int i;
        bool hasPink  = false;
        bool hasAlpha = false;

        for (i = 0; i < mSurface->w * mSurface->h; ++i) {
            if (((unsigned int*)mSurface->pixels)[i] == SDL_MapRGB(mSurface->format, 255, 0, 255)) {
                hasPink = true;
                break;
            }
        }

        for (i = 0; i < mSurface->w * mSurface->h; ++i) {
            Uint8 r, g, b, a;

            SDL_GetRGBA(((unsigned int*)mSurface->pixels)[i], mSurface->format, &r, &g, &b, &a);

            if (a != 255) {
                hasAlpha = true;
                break;
            }
        }

        SDL_Surface* tmp = SDL_ConvertSurface(mSurface, &format, 0);
        SDL_FreeSurface(mSurface);
        mSurface = NULL;

        if (tmp == NULL) {
            fcn::throwException(
                ("Unable to convert image to display format."),
                static_cast<char const *>(__FUNCTION__),
                __FILE__,
                __LINE__);
        }

        if (hasPink) {
            SDL_SetColorKey(tmp, SDL_TRUE, SDL_MapRGB(tmp->format, 255, 0, 255));
        }
        if (hasAlpha) {
            // I'm not sure about this, maybe we should change
            // SDL_SetSurfaceBlendMode() instead e.g. to disable alpha blending
            // if hasAlpha is false.

            // SDL_SetAlpha(tmp, SDL_SRCALPHA, 255);
            SDL_SetSurfaceAlphaMod(tmp, 255);
        }

        mSurface = tmp;
    }

    void SDLImage::free()
    {
        SDL_FreeSurface(mSurface);
    }
} // namespace fcn
