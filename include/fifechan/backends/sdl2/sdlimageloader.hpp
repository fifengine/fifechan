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

    /**
     * SDL implementation of ImageLoader.
     */
    class FIFEGUI_EXT_API SDLImageLoader : public ImageLoader
    {
    public:
        virtual Image* load(std::string const & filename, bool convertToDisplayFormat = true);
        SDL_PixelFormat const & getSDLPixelFormat();
        void setSDLPixelFormat(SDL_PixelFormat const & format);
        void setRenderer(SDL_Renderer* renderer);

    protected:
        virtual SDL_Surface* loadSDLSurface(std::string const & filename);
        virtual SDL_Texture* loadSDLTexture(std::string const & filename);
        virtual SDL_Surface* convertToStandardFormat(SDL_Surface* surface);

    private:
        SDL_PixelFormat mPixelFormat;
        SDL_Renderer* mRenderer;
    };
} // namespace fcn

namespace fcn::sdl2
{
    using ImageLoader = fcn::SDLImageLoader;
}

#endif // INCLUDE_FIFECHAN_BACKENDS_SDL_SDLIMAGELOADER_HPP_
