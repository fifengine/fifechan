// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

// Corresponding header include
#include "fifechan/backends/sdl3/imageloader.hpp"

// Standard library includes
#include <filesystem>
#include <string>

// Third-party library includes
#include <SDL3_image/SDL_image.h>

// Project headers (subdirs before local)
#include "fifechan/backends/sdl3/image.hpp"
#include "fifechan/exception.hpp"

namespace fcn::sdl3
{
    ImageLoader::ImageLoader() = default;

    namespace
    {
        std::string resolveFromExecutableDirectory(std::string const & filename)
        {
            std::filesystem::path const requestedPath(filename);
            if (requestedPath.is_absolute()) {
                return filename;
            }

            char const * basePathRaw = SDL_GetBasePath();
            if (basePathRaw == nullptr) {
                return filename;
            }
            std::filesystem::path const candidate = std::filesystem::path(basePathRaw) / requestedPath;
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
        SDL_DestroySurface(loadedSurface);

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

        // Convert the original surface to the standard format
        auto* converted = SDL_ConvertSurface(surface, SDL_PIXELFORMAT_RGBA8888);

        if (converted == nullptr) {
            return nullptr;
        }

        // Check if the surface already has a color key set (e.g., from the original surface)
        // SDL_ConvertSurface should preserve color key information
        bool hasPink    = false;
        Uint32 colorKey = 0;

        if (SDL_SurfaceHasColorKey(converted)) {
            if (!SDL_GetSurfaceColorKey(converted, &colorKey)) {
                // For SDL_PIXELFORMAT_RGBA8888, the color key is stored as a 32-bit value
                // Extract RGB components (assuming the color key was set with SDL_MapSurfaceRGB)
                Uint8 r = (colorKey >> 16) & 0xFF; // R is typically in bits 16-23 for RGBA8888
                Uint8 g = (colorKey >> 8) & 0xFF;  // G is typically in bits 8-15
                Uint8 b = colorKey & 0xFF;         // B is typically in bits 0-7
                if (r == 255 && g == 0 && b == 255) {
                    hasPink = true;
                }
            }
        }

        // Fallback: if no color key set, scan for magenta pixels (O(n²) - rare case)
        if (!hasPink) {
            for (int y = 0; y < converted->h && !hasPink; ++y) {
                for (int x = 0; x < converted->w && !hasPink; ++x) {
                    uint8_t r{};
                    uint8_t g{};
                    uint8_t b{};
                    uint8_t a{};

                    SDL_ReadSurfacePixel(converted, x, y, &r, &g, &b, &a);

                    if (r == 255 && g == 0 && b == 255) {
                        hasPink = true;
                    }
                }
            }
        }

        if (hasPink) {
            SDL_SetSurfaceColorKey(converted, true, SDL_MapSurfaceRGB(converted, 255, 0, 255));
            SDL_SetSurfaceRLE(converted, true);
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
} // namespace fcn::sdl3
