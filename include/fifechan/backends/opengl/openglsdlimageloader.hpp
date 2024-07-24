// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_BACKENDS_OPENGL_OPENGLSDLIMAGELOADER_HPP_
#define INCLUDE_FIFECHAN_BACKENDS_OPENGL_OPENGLSDLIMAGELOADER_HPP_

#include <fifechan/exception.hpp>
#include <fifechan/opengl/openglimage.hpp>
#include <fifechan/sdl/sdlimageloader.hpp>

#include <string>

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

        virtual Image* load(std::string const & filename, bool convertToDisplayFormat = true)
        {
            SDL_Surface* loadedSurface = loadSDLSurface(filename);

            if (loadedSurface == NULL) {
                std::string msg = std::string("Unable to load image file: " + filename);
                fcn::throwException(msg, static_cast<char const *>(__FUNCTION__), __FILE__, __LINE__);
            }

            SDL_Surface* surface = convertToStandardFormat(loadedSurface);
            SDL_FreeSurface(loadedSurface);

            if (surface == NULL) {
                std::string msg = std::string("Not enough memory to load: " + filename);
                fcn::throwException(msg, static_cast<char const *>(__FUNCTION__), __FILE__, __LINE__);
            }

            OpenGLImage* image =
                new OpenGLImage((unsigned int*)surface->pixels, surface->w, surface->h, convertToDisplayFormat);
            SDL_FreeSurface(surface);

            return image;
        }
    };
} // namespace fcn

#endif // INCLUDE_FIFECHAN_BACKENDS_OPENGL_OPENGLSDLIMAGELOADER_HPP_
