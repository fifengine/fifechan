// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#include <string>

#include <SDL2/SDL_render.h>

#include "fifechan/backends/sdl2/image.hpp"
#include "fifechan/backends/sdl2/imageloader.hpp"
#include "fifechan/backends/sdl2/pixel.hpp"
#include "fifechan/exception.hpp"

namespace fcn::sdl2
{
    SDLImage::SDLImage(SDL_Surface* surface, bool autoFree, SDL_Renderer* renderer) :
        mAutoFree(autoFree), mRenderer(renderer)
    {
        if (renderer != nullptr && surface != nullptr) {
            mTexture = SDL_CreateTextureFromSurface(renderer, surface);
            if (mTexture == nullptr) {
                throwException(std::string("Failed to create texture: ") + SDL_GetError());
            }
            SDL_SetTextureBlendMode(mTexture, SDL_BLENDMODE_BLEND);
#if SDL_VERSION_ATLEAST(2, 0, 12)
            SDL_SetTextureScaleMode(mTexture, SDL_ScaleModeNearest);
#endif

            int w, h;
            SDL_QueryTexture(mTexture, nullptr, nullptr, &w, &h);
            mTransientSurface = SDL_CreateRGBSurfaceWithFormat(0, w, h, 32, SDL_PIXELFORMAT_RGBA8888);
            if (mTransientSurface == nullptr) {
                throwException(std::string("Failed to create transient surface: ") + SDL_GetError());
            }
            SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA8888, 0);
            if (mTransientSurface->format != surface->format) {
                SDL_Surface* converted = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA8888, 0);
                if (converted) {
                    SDL_BlitScaled(converted, nullptr, mTransientSurface, nullptr);
                    SDL_FreeSurface(converted);
                }
            } else {
                SDL_BlitScaled(surface, nullptr, mTransientSurface, nullptr);
            }

            if (autoFree) {
                SDL_FreeSurface(surface);
            }
        } else if (autoFree && surface != nullptr) {
            SDL_FreeSurface(surface);
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
        return mTransientSurface;
    }

    SDL_Texture* SDLImage::getTexture() const
    {
        return mTexture;
    }

    int SDLImage::getWidth() const
    {
        if (mTexture == nullptr) {
            throwException("Trying to get the width of a non loaded image.");
        }

        int w, h;
        SDL_QueryTexture(mTexture, nullptr, nullptr, &w, &h);
        return w;
    }

    int SDLImage::getHeight() const
    {
        if (mTexture == nullptr) {
            throwException("Trying to get the height of a non loaded image.");
        }

        int w, h;
        SDL_QueryTexture(mTexture, nullptr, nullptr, &w, &h);
        return h;
    }

    Color SDLImage::getPixel(int x, int y)
    {
        if (mTransientSurface == nullptr) {
            throwException("Trying to get a pixel from a non loaded image.");
        }

        return SDLgetPixel(mTransientSurface, x, y);
    }

    void SDLImage::putPixel(int x, int y, Color const & color)
    {
        if (mTransientSurface == nullptr) {
            throwException("Trying to put a pixel in a non loaded image.");
        }

        SDLputPixel(mTransientSurface, x, y, color);
    }

    void SDLImage::convertToDisplayFormat()
    {
        if (mTexture == nullptr) {
            throwException("Trying to convert a non loaded image to display format.");
        }
    }

    void SDLImage::free()
    {
        SDL_FreeSurface(mTransientSurface);
        mTransientSurface = nullptr;
        SDL_DestroyTexture(mTexture);
        mTexture = nullptr;
    }
} // namespace fcn::sdl2
