// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef FCN_SDLIMAGELOADER_HPP
#define FCN_SDLIMAGELOADER_HPP

#include "fifechan/imageloader.hpp"
#include "fifechan/platform.hpp"

#include "SDL.h"

namespace fcn
{
    class Image;

    /**
     * SDL implementation of ImageLoader.
     */
    class FIFEGUI_EXT_API SDLImageLoader : public ImageLoader
    {
    public:
        // Inherited from ImageLoader

        virtual Image* load(std::string const & filename, bool convertToDisplayFormat = true);
        SDL_PixelFormat const & getSDLPixelFormat();
        void setSDLPixelFormat(SDL_PixelFormat const & format);

    protected:
        virtual SDL_Surface* loadSDLSurface(std::string const & filename);
        virtual SDL_Surface* convertToStandardFormat(SDL_Surface* surface);

    private:
        SDL_PixelFormat mPixelFormat;
    };
} // namespace fcn

#endif // end FCN_SDLIMAGELOADER_HPP
