// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#include "fifechan/backends/sdl2/image.hpp"

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>

#include <string>

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

            Uint32 colorKey;
            if (SDL_GetColorKey(surface, &colorKey) == 0) {
                // The color key exists. Disable RLE first to access raw pixels.
                SDL_SetSurfaceRLE(surface, 0);

                if (SDL_LockSurface(surface) == 0) {
                    Uint32* pixels       = static_cast<Uint32*>(surface->pixels);
                    int const pixelCount = surface->w * surface->h;

                    // Get format info for RGBA extraction
                    SDL_PixelFormat const * fmt = surface->format;
                    Uint32 const rmask          = fmt->Rmask;
                    Uint32 const gmask          = fmt->Gmask;
                    Uint32 const bmask          = fmt->Bmask;
                    Uint32 const amask          = fmt->Amask;

                    // Extract RGBA, check for magenta and set alpha to 0 if found.
                    for (int i = 0; i < pixelCount; ++i) {
                        // Extract RGBA
                        Uint8 r = (pixels[i] & rmask) >> fmt->Rshift;
                        Uint8 g = (pixels[i] & gmask) >> fmt->Gshift;
                        Uint8 b = (pixels[i] & bmask) >> fmt->Bshift;

                        if (r == 255 && g == 0 && b == 255) {
                            pixels[i] &= ~amask;
                        }
                    }
                    SDL_UnlockSurface(surface);
                }

                // Clear color key as alpha is now used for transparency.
                SDL_SetColorKey(surface, 0, 0);
            }
            // --- End Color Key Conversion ---

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
            SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA8888, 0);
            if (mTransientSurface->format != surface->format) {
                SDL_Surface* converted = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA8888, 0);
                if (converted != nullptr) {
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

    Image::~Image()
    {
        if (mAutoFree) {
            free();
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
    }

    void Image::free()
    {
        SDL_FreeSurface(mTransientSurface);
        mTransientSurface = nullptr;
        SDL_DestroyTexture(mTexture);
        mTexture = nullptr;
    }
} // namespace fcn::sdl2
