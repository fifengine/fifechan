// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_BACKENDS_OPENGL_SDLIMAGELOADER_HPP_
#define INCLUDE_FIFECHAN_BACKENDS_OPENGL_SDLIMAGELOADER_HPP_

// Standard library includes
#include <algorithm>
#include <span>
#include <string>
#include <vector>

// Platform config include
#include "fifechan/platform.hpp"

// Project headers (subdirs before local)
#include <fifechan/backends/opengl/image.hpp>
#include <fifechan/backends/sdl3/imageloader.hpp>
#include <fifechan/exception.hpp>

namespace fcn::opengl
{
    /**
     * OpenGL ImageLoader that loads images with SDL.
     */
    class ImageLoader : public fcn::sdl3::ImageLoader
    {
        public:
            fcn::Image* load(std::string const & filename, bool convertToDisplayFormat) override
            {
                SDL_Surface* loadedSurface = loadSDLSurface(filename);

                if (loadedSurface == nullptr) {
                    std::string const msg = "Unable to load image file: " + filename;
                    throwException(msg);
                }

                SDL_Surface* surface = convertToStandardFormat(loadedSurface);
                SDL_DestroySurface(loadedSurface);

                if (surface == nullptr) {
                    std::string const msg = "Not enough memory to load: " + filename;
                    throwException(msg);
                }

                std::vector<unsigned int> packedPixels(
                    static_cast<size_t>(surface->w) * static_cast<size_t>(surface->h));

                // Read pixels using SDL_ReadSurfacePixel to get correct R,G,B,A
                for (int y = 0; y < surface->h; ++y) {
                    for (int x = 0; x < surface->w; ++x) {
                        unsigned char r = 0;
                        unsigned char g = 0;
                        unsigned char b = 0;
                        unsigned char a = 0;
                        SDL_ReadSurfacePixel(surface, x, y, &r, &g, &b, &a);
                        // Pack as R in bits 0-7, G in 8-15, B in 16-23, A in 24-31
                        size_t const idx =
                            static_cast<size_t>(x) + (static_cast<size_t>(y) * static_cast<size_t>(surface->w));
                        packedPixels.at(idx) = r | (g << 8) | (b << 16) | (a << 24);
                    }
                }
                fcn::Image* image =
                    new fcn::opengl::Image(packedPixels, surface->w, surface->h, convertToDisplayFormat);
                SDL_DestroySurface(surface);

                return image;
            }
    };
} // namespace fcn::opengl

#endif // INCLUDE_FIFECHAN_BACKENDS_OPENGL_SDLIMAGELOADER_HPP_
