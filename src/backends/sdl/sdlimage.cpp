// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#include "fifechan/backends/sdl/sdlimage.hpp"

#include "fifechan/backends/sdl/sdlimageloader.hpp"
#include "fifechan/backends/sdl/sdlpixel.hpp"
#include "fifechan/exception.hpp"

namespace fcn
{
    SDLImage::SDLImage(SDL_Surface* surface, bool autoFree, SDL_Renderer* renderer)
    {
        mAutoFree = autoFree;
        mSurface  = surface;
        mRenderer = renderer;
        if (renderer) {
            mTexture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_SetTextureBlendMode(mTexture, SDL_BLENDMODE_BLEND);
        }
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

    SDL_Texture* SDLImage::getTexture() const
    {
        return mTexture;
    }

    int SDLImage::getWidth() const
    {
        if (mSurface == nullptr) {
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
        if (mSurface == nullptr) {
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
        if (mSurface == nullptr) {
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
        if (mSurface == nullptr) {
            fcn::throwException(
                ("Trying to convert a non loaded image to display format."),
                static_cast<char const *>(__FUNCTION__),
                __FILE__,
                __LINE__);
        }

        int i;
        bool hasPink = false;

        unsigned int surfaceMask = SDL_PIXELFORMAT_RGBX8888;

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
                surfaceMask = SDL_PIXELFORMAT_RGBA8888;
                break;
            }
        }

        SDL_Surface* tmp = SDL_ConvertSurfaceFormat(mSurface, surfaceMask, 0);
        SDL_FreeSurface(mSurface);
        mSurface = NULL;

        if (hasPink) {
            SDL_SetColorKey(tmp, SDL_TRUE, SDL_MapRGB(tmp->format, 255, 0, 255));
        }

        if (surfaceMask == SDL_PIXELFORMAT_RGBA8888)
            SDL_SetSurfaceAlphaMod(tmp, 255);

        mSurface = tmp;

        if (mRenderer) {
            SDL_Texture* tmpTexture = SDL_CreateTextureFromSurface(mRenderer, tmp);
            SDL_SetTextureBlendMode(tmpTexture, SDL_BLENDMODE_BLEND);
            SDL_DestroyTexture(mTexture);
            mTexture = tmpTexture;
        }
    }

    void SDLImage::free()
    {
        SDL_FreeSurface(mSurface);
        SDL_DestroyTexture(mTexture);
    }
} // namespace fcn
