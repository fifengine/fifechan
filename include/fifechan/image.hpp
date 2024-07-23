// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_IMAGE_HPP_
#define INCLUDE_FIFECHAN_IMAGE_HPP_

#include <string>

#include "fifechan/platform.hpp"

namespace fcn
{
    class Color;
    class ImageLoader;

    /**
     * Holds an image. To be able to use this class you must first set an
     * ImageLoader in Image by calling
     * @code Image::setImageLoader(myImageLoader) @endcode
     * The function is static. If this is not done, the constructor taking a
     * filename will throw an exception. The ImageLoader you use must be
     * compatible with the Graphics object you use.
     *
     * EXAMPLE: If you use SDLGraphics you should use SDLImageLoader.
     *          Otherwise your program might crash in a most bizarre way.
     * @see AllegroImageLoader, HGEImageLoader, OpenLayerImageLoader,
     *      OpenGLAllegroImageLoader, OpenGLSDLImageLoader, SDLImageLoader
     */
    class FIFEGUI_API Image
    {
    public:
        Image();

        virtual ~Image();

        /**
         * Loads an image by using the class' image laoder. All image loaders implemented
         * in Fifechan return a newly instantiated image which must be deleted in
         * order to avoid a memory leak.
         *
         * NOTE: The functions getPixel and putPixel are only guaranteed to work
         *       before an image has been converted to display format.
         *
         * @param filename The file to load.
         * @param convertToDisplayFormat True if the image should be converted
         *                               to display, false otherwise.
         */
        static Image* load(std::string const & filename, bool convertToDisplayFormat = true);

        /**
         * Gets the image loader used for loading images.
         *
         * @return The image loader used for loading images.
         * @see setImageLoader, AllegroImageLoader, HGEImageLoader,
         *      OpenLayerImageLoader, OpenGLAllegroImageLoader,
         *      OpenGLSDLImageLoader, SDLImageLoader
         */
        static ImageLoader* getImageLoader();

        /**
         * Sets the ImageLoader to be used for loading images.
         *
         * IMPORTANT: The image loader is static and MUST be set before
         *            loading images!
         *
         * @param imageLoader The image loader to be used for loading images.
         * @see getImageLoader, AllegroImageLoader, HGEImageLoader,
         *      OpenLayerImageLoader, OpenGLAllegroImageLoader,
         *      OpenGLSDLImageLoader, SDLImageLoader
         */
        static void setImageLoader(ImageLoader* imageLoader);

        /**
         * Frees an image.
         *
         */
        virtual void free() = 0;

        /**
         * Gets the width of the image.
         *
         * @return The width of the image.
         *
         */
        virtual int getWidth() const = 0;

        /**
         * Gets the height of the image.
         *
         * @return The height of the image.
         *
         */
        virtual int getHeight() const = 0;

        /**
         * Gets the color of a pixel at coordinate (x, y) in the image.
         *
         * IMPORTANT: Only guaranteed to work before the image has been
         *            converted to display format.
         *
         * @param x The x coordinate.
         * @param y The y coordinate.
         * @return The color of the pixel.
         *
         */
        virtual Color getPixel(int x, int y) = 0;

        /**
         * Puts a pixel with a certain color at coordinate (x, y).
         *
         * @param x The x coordinate.
         * @param y The y coordinate.
         * @param color The color of the pixel to put.
         */
        virtual void putPixel(int x, int y, Color const & color) = 0;

        /**
         * Converts the image, if possible, to display format.
         *
         * IMPORTANT: Only guaranteed to work before the image has been
         *            converted to display format.
         */
        virtual void convertToDisplayFormat() = 0;

    protected:
        /**
         * Holds the image loader to be used when loading images.
         */
        static ImageLoader* mImageLoader;
    };
} // namespace fcn

#endif // INCLUDE_FIFECHAN_IMAGE_HPP_
