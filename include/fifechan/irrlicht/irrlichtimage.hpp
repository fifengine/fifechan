// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef FCN_IRRLICHTIMAGE_HPP
#define FCN_IRRLICHTIMAGE_HPP

#include "irrlicht.h"

#include <string>

#include "fifechan/color.hpp"
#include "fifechan/platform.hpp"
#include "fifechan/image.hpp"

namespace fcn
{
    /**
     * Irrlicht implementation of Image.
     */
    class FIFEGUI_EXT_API IrrlichtImage : public Image
    {
    public:
        /**
         * Constructor. Load an image from an Irrlicht IImage.
         *
         * NOTE: The functions getPixel and putPixel will only work
         *       before an image has been converted to display format.
         *
         * @param image the image from which to load.
         * @param driver the Irrlicht video driver to use when converting the image to display format.
         * @param name the filename of the image.
         * @param autoFree true if the image should automatically be deleted.
         * @param convertToDisplayFormat true if the image should be converted to display format.
         */
        IrrlichtImage(
            irr::video::IImage* image,
            irr::video::IVideoDriver* driver,
            std::string const & name,
            bool autoFree,
            bool convertToDisplayFormat);

        /**
         * Destructor.
         */
        virtual ~IrrlichtImage();

        /**
         * Gets the Irrlicht ITexture for the image.
         *
         * @return the Irrlicht ITexture for the image.
         */
        virtual irr::video::ITexture* getTexture() const;

        // Inherited from Image

        virtual void free();

        virtual int getWidth() const;

        virtual int getHeight() const;

        virtual Color getPixel(int x, int y);

        virtual void putPixel(int x, int y, Color const & color);

        virtual void convertToDisplayFormat();

    protected:
        /**
         * Holds the Irrlicht IImage. This Irrlicht IImage is NULL after the image
         * has been converted to display format.
         */
        irr::video::IImage* mImage;

        /**
         * Holds the texture of the image. This texture isn't initialized until
         * the image is converted to display format.
         */
        irr::video::ITexture* mTexture;

        /**
         * Holds the D3D device used when converting the image to display format.
         */
        irr::video::IVideoDriver* mDriver;

        /**
         * Holds the filename of the image.
         */
        std::string mName;

        /**
         * True if the image should be auto freed.
         */
        bool mAutoFree;
    };
} // namespace fcn

#endif // end FCN_IRRLICHTIMAGE_HPP
