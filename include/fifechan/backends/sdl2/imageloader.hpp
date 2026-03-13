// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_BACKENDS_SDL_SDLIMAGELOADER_HPP_
#define INCLUDE_FIFECHAN_BACKENDS_SDL_SDLIMAGELOADER_HPP_

#include <SDL2/SDL.h>

#include <string>

#include "fifechan/imageloader.hpp"
#include "fifechan/platform.hpp"

namespace fcn
{
    class Image;
}

namespace fcn::sdl2
{

    /**
     * SDL implementation of ImageLoader.
     */
    class FIFEGUI_EXT_API ImageLoader : public fcn::ImageLoader
    {
    public:
        using fcn::ImageLoader::load;

        /** Load an image from `filename`. */
        fcn::Image* load(std::string const & filename, bool convertToDisplayFormat) override;

        /** Return the current SDL pixel format used for conversions. */
        SDL_PixelFormat const & getSDLPixelFormat();

        /** Set the SDL pixel format used for conversions. */
        void setSDLPixelFormat(SDL_PixelFormat const & format);

        /** Set the SDL renderer used when creating textures. */
        void setRenderer(SDL_Renderer* renderer);

    protected:
        /** Load an SDL_Surface from disk (internal). */
        virtual SDL_Surface* loadSDLSurface(std::string const & filename);

        /** Load an SDL_Texture from disk (internal). */
        virtual SDL_Texture* loadSDLTexture(std::string const & filename);

        /** Convert a surface to a standard internal format (internal). */
        virtual SDL_Surface* convertToStandardFormat(SDL_Surface* surface);

    private:
        /** Pixel format used for conversions and blending. */
        SDL_PixelFormat mPixelFormat;

        /** Optional renderer used when creating textures. */
        SDL_Renderer* mRenderer;
    };
} // namespace fcn::sdl2

#endif // INCLUDE_FIFECHAN_BACKENDS_SDL_SDLIMAGELOADER_HPP_
