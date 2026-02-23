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
        Uint32 rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
        rmask = 0xff000000;
        gmask = 0x00ff0000;
        bmask = 0x0000ff00;
        amask = 0x000000ff;
#else
        rmask = 0x000000ff;
        gmask = 0x0000ff00;
        bmask = 0x00ff0000;
        amask = 0xff000000;
#endif

        SDL_Surface* colorSurface = SDL_CreateRGBSurface(SDL_SWSURFACE, 0, 0, 32, rmask, gmask, bmask, amask);

        SDL_Surface* tmp = nullptr;

        if (colorSurface != nullptr) {
            tmp = SDL_ConvertSurface(surface, colorSurface->format, SDL_SWSURFACE);
            SDL_FreeSurface(colorSurface);
        }

        return tmp;
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
