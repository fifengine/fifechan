// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_IMAGELOADER_HPP_
#define INCLUDE_FIFECHAN_IMAGELOADER_HPP_

#include <string>

#include "fifechan/platform.hpp"

namespace fcn
{
    class Image;

    /**
     * Abstract interface for loading image assets.
     *
     * FifeGUI contains implementations of ImageLoader for common
     * libraries like the SDL library.
     * To make FifeGUI usable with other libraries, an ImageLoader
     * class must be implemented.
     *
     * To make FifeGUIuse an image loader, the image loader needs
     * to be passed to the Image class using the static method
     * Image::setImageLoader.
     *
     * @see Image::setImageLoader, Image::getImageLoader,
     *      OpenGLSDLImageLoader, SDLImageLoader
     *
     * @ingroup graphics
     */
    class FIFEGUI_API ImageLoader
    {
    public:
        virtual ~ImageLoader() = default;

        ImageLoader(ImageLoader const &)            = default;
        ImageLoader& operator=(ImageLoader const &) = default;
        ImageLoader(ImageLoader&&)                  = default;
        ImageLoader& operator=(ImageLoader&&)       = default;

    protected:
        ImageLoader() = default;

    public:
        /**
         * Loads an image.
         *
         * The single-argument overload forwards to the
         * two-argument overload requesting conversion to display format.
         *
         * NOTE: The functions Image::getPixel and Image::putPixel
         *       are only guaranteed to work before an image has
         *       been converted to display format.
         *
         * @param filename The filename of the image to load.
         */
        Image* load(std::string const & filename)
        {
            return load(filename, true);
        }

        /**
         * Loads an image, optionally converting it to display format.
         *
         * @param filename The filename of the image to load.
         * @param convertToDisplayFormat True if the image should be converted
         *                               to display format, false otherwise.
         */
        virtual Image* load(std::string const & filename, bool convertToDisplayFormat) = 0;
    };
} // namespace fcn

#endif // INCLUDE_FIFECHAN_IMAGELOADER_HPP_
