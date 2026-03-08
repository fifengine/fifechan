// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_BACKENDS_OPENGL_OPENGLSDLIMAGELOADER_HPP_
#define INCLUDE_FIFECHAN_BACKENDS_OPENGL_OPENGLSDLIMAGELOADER_HPP_

#include <fifechan/backends/opengl/openglimage.hpp>
#include <fifechan/backends/sdl2/sdlimageloader.hpp>
#include <fifechan/exception.hpp>

#include <algorithm>
#include <span>
#include <string>
#include <vector>

namespace fcn
{
    class Image;

    /**
     * OpenGL ImageLoader that loads images with SDL.
     */
    class OpenGLSDLImageLoader : public SDLImageLoader
    {
    public:
        // Inherited from ImageLoader

        using SDLImageLoader::load;

        Image* load(std::string const & filename, bool convertToDisplayFormat) override
        {
            SDL_Surface* loadedSurface = loadSDLSurface(filename);

            if (loadedSurface == nullptr) {
                std::string const msg = "Unable to load image file: " + filename;
                throwException(msg);
            }

            SDL_Surface* surface = convertToStandardFormat(loadedSurface);
            SDL_FreeSurface(loadedSurface);

            if (surface == nullptr) {
                std::string const msg = "Not enough memory to load: " + filename;
                throwException(msg);
            }

            std::vector<unsigned int> packedPixels(static_cast<size_t>(surface->w) * static_cast<size_t>(surface->h));
            unsigned int const * srcPixels = static_cast<unsigned int const *>(surface->pixels);
            size_t const srcPitchPixels =
                static_cast<size_t>(static_cast<size_t>(surface->pitch) / sizeof(unsigned int));

            std::span<unsigned int const> const srcSpan(srcPixels, srcPitchPixels * static_cast<size_t>(surface->h));
            std::span<unsigned int> const dstSpan(packedPixels.data(), packedPixels.size());

            for (int y = 0; y < surface->h; ++y) {
                size_t const rowIndex = static_cast<size_t>(y);
                auto srcIt            = srcSpan.begin() + (rowIndex * srcPitchPixels);
                auto dstIt            = dstSpan.begin() + (rowIndex * static_cast<size_t>(surface->w));
                std::copy_n(srcIt, static_cast<size_t>(surface->w), dstIt);
            }

            Image* image = new OpenGLImage(packedPixels, surface->w, surface->h, convertToDisplayFormat);
            SDL_FreeSurface(surface);

            return image;
        }
    };
} // namespace fcn

#endif // INCLUDE_FIFECHAN_BACKENDS_OPENGL_OPENGLSDLIMAGELOADER_HPP_
