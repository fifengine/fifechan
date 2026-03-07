// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_BACKENDS_OPENGL_OPENGLSDLIMAGELOADER_HPP_
#define INCLUDE_FIFECHAN_BACKENDS_OPENGL_OPENGLSDLIMAGELOADER_HPP_

#include <fifechan/backends/opengl/openglimage.hpp>
#include <fifechan/backends/sdl2/sdlimageloader.hpp>
#include <fifechan/exception.hpp>

#include <algorithm>
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
                std::string msg = std::string("Unable to load image file: " + filename);
                throwException(msg);
            }

            SDL_Surface* surface = convertToStandardFormat(loadedSurface);
            SDL_FreeSurface(loadedSurface);

            if (surface == nullptr) {
                std::string msg = std::string("Not enough memory to load: " + filename);
                throwException(msg);
            }

            std::vector<unsigned int> packedPixels(static_cast<size_t>(surface->w) * static_cast<size_t>(surface->h));
            unsigned int const * srcPixels = static_cast<unsigned int const *>(surface->pixels);
            int const srcPitchPixels       = surface->pitch / static_cast<int>(sizeof(unsigned int));

            for (int y = 0; y < surface->h; ++y) {
                unsigned int* dstRow = packedPixels.data() + static_cast<size_t>(y) * static_cast<size_t>(surface->w);
                unsigned int const * srcRow = srcPixels + static_cast<size_t>(y) * static_cast<size_t>(srcPitchPixels);
                std::copy(srcRow, srcRow + surface->w, dstRow);
            }

            Image* image = new OpenGLImage(packedPixels, surface->w, surface->h, convertToDisplayFormat);
            SDL_FreeSurface(surface);

            return image;
        }
    };
} // namespace fcn

#endif // INCLUDE_FIFECHAN_BACKENDS_OPENGL_OPENGLSDLIMAGELOADER_HPP_
