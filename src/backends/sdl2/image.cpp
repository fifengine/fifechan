// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

// Corresponding header include
#include "fifechan/backends/sdl2/image.hpp"

// Standard library includes
#include <string>

// Third-party library includes
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>

// Project headers (subdirs before local)
#include "fifechan/backends/sdl2/imageloader.hpp"
#include "fifechan/backends/sdl2/pixel.hpp"
#include "fifechan/exception.hpp"

namespace fcn::sdl2
{
    Image::Image(SDL_Surface* surface, bool autoFree, SDL_Renderer* renderer) : mAutoFree(autoFree), mRenderer(renderer)
    {
        if (renderer != nullptr && surface != nullptr) {

            // Color Key to Alpha Conversion
            //
            // Convert magenta (255,0,255) color key to proper alpha channel.
            // Why? SDL_CreateTextureFromSurface doesn't automatically convert
            // color-keyed surfaces to alpha transparency.

            // The color key exists. Disable RLE first to access raw pixels.
            SDL_SetSurfaceRLE(surface, 0);

            if (surface->format->BitsPerPixel != 32) {
                SDL_Surface* converted = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA8888, 0);
                if (converted != nullptr) {
                    if (autoFree) {
                        SDL_FreeSurface(surface);
                    }
                    surface = converted;
                }
            }

            // Set magenta as color key only if not already set by ImageLoader
            Uint32 existingKey = 0;
            if (SDL_GetColorKey(surface, &existingKey) == 0) {
                // Color key already set - preserve it (from ImageLoader)
            } else {
                // No color key set - don't add one in constructor (for font images)
            }

            mTexture = SDL_CreateTextureFromSurface(renderer, surface);
            if (mTexture == nullptr) {
                throwException(std::string("Failed to create texture: ") + SDL_GetError());
            }

            SDL_SetTextureBlendMode(mTexture, SDL_BLENDMODE_BLEND);
#if SDL_VERSION_ATLEAST(2, 0, 12)
            SDL_SetTextureScaleMode(mTexture, SDL_ScaleModeNearest);
#endif

            int w{};
            int h{};
            SDL_QueryTexture(mTexture, nullptr, nullptr, &w, &h);
            mTransientSurface = SDL_CreateRGBSurfaceWithFormat(0, w, h, 32, SDL_PIXELFORMAT_RGBA8888);
            if (mTransientSurface == nullptr) {
                throwException(std::string("Failed to create transient surface: ") + SDL_GetError());
            }
            if (mTransientSurface->format != surface->format) {
                SDL_Surface* converted = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA8888, 0);
                if (converted != nullptr) {
                    SDL_BlitScaled(converted, nullptr, mTransientSurface, nullptr);
                    SDL_FreeSurface(converted);
                }
            } else {
                SDL_BlitScaled(surface, nullptr, mTransientSurface, nullptr);
            }

            Uint32 transientKey = 0;
            if (SDL_GetColorKey(mTransientSurface, &transientKey) == 0) {
                // Color key already set - preserve it (from ImageLoader)
            } else {
                // No color key set - don't add one (for font images)
            }
            SDL_SetSurfaceRLE(mTransientSurface, 0);

            if (autoFree) {
                SDL_FreeSurface(surface);
            }
        } else if (autoFree && surface != nullptr) {
            SDL_FreeSurface(surface);
        }
    }

    Image::~Image()
    {
        if (mAutoFree) {
            SDL_FreeSurface(mTransientSurface);
            mTransientSurface = nullptr;
            SDL_DestroyTexture(mTexture);
            mTexture = nullptr;
        }
    }

    SDL_Surface* Image::getSurface() const
    {
        return mTransientSurface;
    }

    SDL_Texture* Image::getTexture() const
    {
        return mTexture;
    }

    int Image::getWidth() const
    {
        if (mTexture == nullptr) {
            throwException("Trying to get the width of a non loaded image.");
        }

        int w{};
        int h{};
        SDL_QueryTexture(mTexture, nullptr, nullptr, &w, &h);
        return w;
    }

    int Image::getHeight() const
    {
        if (mTexture == nullptr) {
            throwException("Trying to get the height of a non loaded image.");
        }

        int w{};
        int h{};
        SDL_QueryTexture(mTexture, nullptr, nullptr, &w, &h);
        return h;
    }

    Color Image::getPixel(int x, int y)
    {
        if (mTransientSurface == nullptr) {
            throwException("Trying to get a pixel from a non loaded image.");
        }

        return SDLgetPixel(mTransientSurface, x, y);
    }

    void Image::putPixel(int x, int y, Color const & color)
    {
        if (mTransientSurface == nullptr) {
            throwException("Trying to put a pixel in a non loaded image.");
        }

        SDLputPixel(mTransientSurface, x, y, color);
    }

    void Image::convertToDisplayFormat()
    {
        if (mTexture == nullptr) {
            throwException("Trying to convert a non loaded image to display format.");
        }

        // Regenerate the texture from the transient surface which has the final pixel data.
        // This ensures the texture reflects any changes made via getPixel/putPixel.
        if (mTransientSurface != nullptr && mRenderer != nullptr) {
            SDL_DestroyTexture(mTexture);
            mTexture = SDL_CreateTextureFromSurface(mRenderer, mTransientSurface);
            if (mTexture == nullptr) {
                throwException(std::string("Failed to create texture in convertToDisplayFormat: ") + SDL_GetError());
            }
            SDL_SetTextureBlendMode(mTexture, SDL_BLENDMODE_BLEND);
#if SDL_VERSION_ATLEAST(2, 0, 12)
            SDL_SetTextureScaleMode(mTexture, SDL_ScaleModeNearest);
#endif
        }
    }

    void Image::free()
    {
        SDL_FreeSurface(mTransientSurface);
        mTransientSurface = nullptr;
        SDL_DestroyTexture(mTexture);
        mTexture = nullptr;
    }
} // namespace fcn::sdl2
