// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

// Corresponding header include
#include "fifechan/backends/sdl3/image.hpp"

// Standard library includes
#include <string>

// Third-party library includes
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

// Project headers (subdirs before local)
#include "fifechan/backends/sdl3/imageloader.hpp"
#include "fifechan/exception.hpp"

namespace fcn::sdl3
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
            SDL_SetSurfaceRLE(surface, false);

            if (SDL_GetPixelFormatDetails(surface->format)->bits_per_pixel != 32) {
                SDL_Surface* converted = SDL_ConvertSurface(surface, SDL_PIXELFORMAT_RGBA8888);
                if (converted != nullptr) {
                    if (autoFree) {
                        SDL_DestroySurface(surface);
                    }
                    surface = converted;
                }
            }

            // Set magenta as color key only if not already set by ImageLoader
            Uint32 existingKey = 0;
            if (SDL_GetSurfaceColorKey(surface, &existingKey) == false) {
                // Color key already set - preserve it (from ImageLoader)
            } else {
                // No color key set - don't add one in constructor (for font images)
            }

            mTexture = SDL_CreateTextureFromSurface(renderer, surface);
            if (mTexture == nullptr) {
                throwException(std::string("Failed to create texture: ") + SDL_GetError());
            }

            SDL_SetTextureBlendMode(mTexture, SDL_BLENDMODE_BLEND);
            SDL_SetTextureScaleMode(mTexture, SDL_SCALEMODE_NEAREST);

            float wFloat = 0;
            float hFloat = 0;
            SDL_GetTextureSize(mTexture, &wFloat, &hFloat);

            int const w = static_cast<int>(wFloat);
            int const h = static_cast<int>(hFloat);

            mTransientSurface = SDL_CreateSurface(w, h, SDL_PIXELFORMAT_RGBA8888);

            if (mTransientSurface == nullptr) {
                throwException(std::string("Failed to create transient surface: ") + SDL_GetError());
            }

            if (mTransientSurface->format != surface->format) {
                SDL_Surface* converted = SDL_ConvertSurface(surface, SDL_PIXELFORMAT_RGBA8888);
                if (converted != nullptr) {
                    SDL_BlitSurface(converted, nullptr, mTransientSurface, nullptr);
                    SDL_DestroySurface(converted);
                }
            } else {
                SDL_BlitSurface(surface, nullptr, mTransientSurface, nullptr);
            }

            Uint32 transientKey = 0;
            if (SDL_GetSurfaceColorKey(mTransientSurface, &transientKey) == false) {
                // Color key already set - preserve it (from ImageLoader)
            } else {
                // No color key set - don't add one (for font images)
            }
            SDL_SetSurfaceRLE(mTransientSurface, false);

            if (autoFree) {
                SDL_DestroySurface(surface);
            }
        } else if (autoFree && surface != nullptr) {
            SDL_DestroySurface(surface);
        }
    }

    Image::~Image()
    {
        if (mAutoFree) {
            SDL_DestroySurface(mTransientSurface);
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

        float wFloat = 0;
        float hFloat = 0;
        SDL_GetTextureSize(mTexture, &wFloat, &hFloat);

        return static_cast<int>(wFloat);
    }

    int Image::getHeight() const
    {
        if (mTexture == nullptr) {
            throwException("Trying to get the height of a non loaded image.");
        }

        float wFloat = 0;
        float hFloat = 0;
        SDL_GetTextureSize(mTexture, &wFloat, &hFloat);

        return static_cast<int>(hFloat);
    }

    Color Image::getPixel(int x, int y)
    {
        if (mTransientSurface == nullptr) {
            throwException("Trying to get a pixel from a non loaded image.");
        }

        unsigned char r = 0;
        unsigned char g = 0;
        unsigned char b = 0;
        unsigned char a = 0;
        SDL_ReadSurfacePixel(mTransientSurface, x, y, &r, &g, &b, &a);

        return {r, g, b, a};
    }

    void Image::putPixel(int x, int y, Color const & color)
    {
        if (mTransientSurface == nullptr) {
            throwException("Trying to put a pixel in a non loaded image.");
        }

        SDL_WriteSurfacePixel(mTransientSurface, x, y, color.r, color.g, color.b, color.a);
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
            SDL_SetTextureScaleMode(mTexture, SDL_SCALEMODE_NEAREST);
        }
    }

    void Image::free()
    {
        SDL_DestroySurface(mTransientSurface);
        mTransientSurface = nullptr;
        SDL_DestroyTexture(mTexture);
        mTexture = nullptr;
    }
} // namespace fcn::sdl3
