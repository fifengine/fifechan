// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#include "fifechan/backends/sdl2/imageloader.hpp"

#include <filesystem>
#include <string>

#include "SDL_image.h"
#include "fifechan/backends/sdl2/image.hpp"
#include "fifechan/exception.hpp"

namespace fcn::sdl2
{
    namespace
    {
        std::string resolveFromExecutableDirectory(std::string const & filename)
        {
            std::filesystem::path const requestedPath(filename);
            if (requestedPath.is_absolute()) {
                return filename;
            }

            char* basePathRaw = SDL_GetBasePath();
            if (basePathRaw == nullptr) {
                return filename;
            }

            std::filesystem::path const candidate = std::filesystem::path(basePathRaw) / requestedPath;
            SDL_free(basePathRaw);
            return candidate.string();
        }
    } // namespace

    fcn::Image* ImageLoader::load(std::string const & filename, bool convertToDisplayFormat)
    {
        SDL_Surface* loadedSurface = loadSDLSurface(filename);

        if (loadedSurface == nullptr) {
            throwException(std::string("Unable to load image file: ") + filename);
        }

        SDL_Surface* surface = convertToStandardFormat(loadedSurface);
        SDL_FreeSurface(loadedSurface);

        if (surface == nullptr) {
            throwException((std::string("Not enough memory to load: ") + filename));
        }

        Image* image = new Image(surface, true, mRenderer);

        if (convertToDisplayFormat) {
            image->convertToDisplayFormat();
        }

        return image;
    }

    void ImageLoader::setRenderer(SDL_Renderer* renderer)
    {
        mRenderer = renderer;
    }

    SDL_Surface* ImageLoader::loadSDLSurface(std::string const & filename)
    {
        SDL_Surface* surface = IMG_Load(filename.c_str());
        if (surface != nullptr) {
            return surface;
        }

        std::string const resolvedPath = resolveFromExecutableDirectory(filename);
        if (resolvedPath == filename) {
            return nullptr;
        }

        return IMG_Load(resolvedPath.c_str());
    }

    SDL_Texture* ImageLoader::loadSDLTexture(std::string const & filename)
    {
        SDL_Texture* texture = IMG_LoadTexture(mRenderer, filename.c_str());
        if (texture != nullptr) {
            return texture;
        }

        std::string const resolvedPath = resolveFromExecutableDirectory(filename);
        if (resolvedPath == filename) {
            return nullptr;
        }

        return IMG_LoadTexture(mRenderer, resolvedPath.c_str());
    }

    SDL_Surface* ImageLoader::convertToStandardFormat(SDL_Surface* surface)
    {
        if (surface == nullptr) {
            return nullptr;
        }

        bool hasPink             = false;
        unsigned int surfaceMask = SDL_PIXELFORMAT_RGBX8888;

        int const pixels = surface->w * surface->h;

        for (int i = 0; i < pixels; ++i) {
            uint8_t r{};
            uint8_t g{};
            uint8_t b{};
            uint8_t a{};

            SDL_GetRGBA(reinterpret_cast<Uint32*>(surface->pixels)[i], surface->format, &r, &g, &b, &a);

            if (r == 255 && g == 0 && b == 255) {
                hasPink = true;
            }

            if (a != 255) {
                surfaceMask = SDL_PIXELFORMAT_RGBA8888;
                break;
            }
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
        uint32_t const rmask = 0x000000ff;
        uint32_t const gmask = 0x0000ff00;
        uint32_t const bmask = 0x00ff0000;
        uint32_t const amask = 0xff000000;
#endif

        // Create a 32-bit color surface to standardize format
        auto* targetFormatSurface = SDL_CreateRGBSurface(0, surface->w, surface->h, 32, rmask, gmask, bmask, amask);

        if (targetFormatSurface == nullptr) {
            return nullptr;
        }

        // Convert the original surface to the standard format
        auto* converted = SDL_ConvertSurface(surface, targetFormatSurface->format, 0);
        SDL_FreeSurface(targetFormatSurface);

        if (converted == nullptr) {
            return nullptr;
        }

        if (hasPink) {
            SDL_SetColorKey(converted, SDL_TRUE, SDL_MapRGB(converted->format, 255, 0, 255));
            SDL_SetSurfaceRLE(converted, 1);
        }

        if (surfaceMask == SDL_PIXELFORMAT_RGBA8888) {
            SDL_SetSurfaceAlphaMod(converted, 255);
        }

        return converted;
    }

    SDL_PixelFormat const & ImageLoader::getSDLPixelFormat()
    {
        return mPixelFormat;
    }

    void ImageLoader::setSDLPixelFormat(SDL_PixelFormat const & format)
    {
        mPixelFormat = format;
    }
} // namespace fcn::sdl2
