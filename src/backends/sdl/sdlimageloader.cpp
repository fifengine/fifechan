// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#include "fifechan/backends/sdl/sdlimageloader.hpp"

#include "SDL_image.h"
#include "fifechan/backends/sdl/sdlimage.hpp"
#include "fifechan/exception.hpp"

namespace fcn
{
    Image* SDLImageLoader::load(std::string const & filename, bool convertToDisplayFormat)
    {
        SDL_Surface* loadedSurface = loadSDLSurface(filename);

        if (loadedSurface == nullptr) {
            fcn::throwException(
                (std::string("Unable to load image file: ") + filename),
                static_cast<char const *>(__FUNCTION__),
                __FILE__,
                __LINE__);
        }

        SDL_Surface* surface = convertToStandardFormat(loadedSurface);
        SDL_FreeSurface(loadedSurface);

        if (surface == nullptr) {
            fcn::throwException(
                (std::string("Not enough memory to load: ") + filename),
                static_cast<char const *>(__FUNCTION__),
                __FILE__,
                __LINE__);
        }

        Image* image = new SDLImage(surface, true);

        if (convertToDisplayFormat) {
            image->convertToDisplayFormat();
        }

        return image;
    }

    void SDLImageLoader::setRenderer(SDL_Renderer* renderer)
    {
        mRenderer = renderer;
    }

    SDL_Surface* SDLImageLoader::loadSDLSurface(std::string const & filename)
    {
        return IMG_Load(filename.c_str());
    }

    SDL_Texture* SDLImageLoader::loadSDLTexture(std::string const & filename)
    {
        return IMG_LoadTexture(mRenderer, filename.c_str());
    }

    SDL_Surface* SDLImageLoader::convertToStandardFormat(SDL_Surface* surface)
    {
        if (surface == nullptr) {
            return nullptr;
        }

        // SDL interprets each pixel as a 32-bit number.
        // We need to mask depending on the endianness (byte order) of the machine.
        // Rmask being 0xFF000000 means the red data is stored in the most significant byte
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
        uint32_t rmask = 0xff000000;
        uint32_t gmask = 0x00ff0000;
        uint32_t bmask = 0x0000ff00;
        uint32_t amask = 0x000000ff;
#else
        uint32_t rmask = 0x000000ff;
        uint32_t gmask = 0x0000ff00;
        uint32_t bmask = 0x00ff0000;
        uint32_t amask = 0xff000000;
#endif

        // Create a 32-bit color surface to standardize format
        auto targetFormatSurface = SDL_CreateRGBSurface(0, surface->w, surface->h, 32, rmask, gmask, bmask, amask);

        if (targetFormatSurface == nullptr) {
            return nullptr;
        }

        // Convert the original surface to the standard format
        auto converted = SDL_ConvertSurface(surface, targetFormatSurface->format, 0);
        SDL_FreeSurface(targetFormatSurface);

        return converted;
    }

    SDL_PixelFormat const & SDLImageLoader::getSDLPixelFormat()
    {
        return mPixelFormat;
    }

    void SDLImageLoader::setSDLPixelFormat(SDL_PixelFormat const & format)
    {
        mPixelFormat = format;
    }
} // namespace fcn
