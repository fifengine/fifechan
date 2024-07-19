// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef FCN_SDLIMAGE_HPP
#define FCN_SDLIMAGE_HPP

#include "SDL.h"

#include <string>

#include "fifechan/color.hpp"
#include "fifechan/platform.hpp"
#include "fifechan/image.hpp"

namespace fcn
{
    /**
     * SDL implementation of Image.
     */
    class FCN_EXTENSION_DECLSPEC SDLImage : public Image
    {
    public:
        /**
         * Constructor. Load an image from an SDL surface.
         *
         * NOTE: The functions getPixel and putPixel are only guaranteed to work
         *       before an image has been converted to display format.
         *
         * @param surface the surface from which to load.
         * @param autoFree true if the surface should automatically be deleted.
         */
        SDLImage(SDL_Surface* surface, bool autoFree);

        /**
         * Destructor.
         */
        virtual ~SDLImage();

        /**
         * Gets the SDL surface for the image.
         *
         * @return the SDL surface for the image.
         */
        virtual SDL_Surface* getSurface() const;

        // Inherited from Image

        virtual void free();

        virtual int getWidth() const;

        virtual int getHeight() const;

        virtual Color getPixel(int x, int y);

        virtual void putPixel(int x, int y, Color const & color);

        virtual void convertToDisplayFormat();

    protected:
        SDL_Surface* mSurface;
        bool mAutoFree;
    };
} // namespace fcn

#endif // end FCN_SDLIMAGE_HPP
