// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef FCN_IMAGELOADER_HPP
#define FCN_IMAGELOADER_HPP

#include <string>

#include "fifechan/platform.hpp"

namespace fcn
{
    class Image;

    /**
     * Abstract class for providing functions for loading images.
     *
     * Guichan contains implementations of ImageLoader for common
     * libraries like the Allegro library, the HGE library,
     * he OpenLayer library, and the SDL library.
     * To make Guichan usable with other libraries, an ImageLoader
     * class must be implemented.
     *
     * To make Guichan use an image loader, the image loader needs
     * to be passed to the Image class using the static method
     * Image::setImageLoader.
     *
     * @see Image::setImageLoader, Image::getImageLoader,
     *      AllegroImageLoader, HGEImageLoader, OpenLayerImageLoader,
     *      OpenGLAllegroImageLoader, OpenGLSDLImageLoader,
     *      SDLImageLoader
     */
    class FIFEGUI_API ImageLoader
    {
    public:
        /**
         * Destructor.
         */
        virtual ~ImageLoader() = default;

        /**
         * Loads an image.
         *
         * NOTE: The functions Image::getPixel and Image::putPixel
         *       are only guaranteed to work before an image has
         *       been converted to display format.
         *
         * @param filename The filename of the image to load.
         * @param convertToDisplayFormat True if the image should be converted
         *                               to display, false otherwise.
         */
        virtual Image* load(std::string const & filename, bool convertToDisplayFormat = true) = 0;
    };
} // namespace fcn

#endif // end FCN_IMAGELOADER_HPP
