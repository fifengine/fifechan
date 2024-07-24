// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#include "fifechan/backends/irrlicht/irrlichtimage.hpp"

#include "fifechan/exception.hpp"

namespace fcn
{
    IrrlichtImage::IrrlichtImage(
        irr::video::IImage* image,
        irr::video::IVideoDriver* driver,
        std::string const & name,
        bool autoFree,
        bool convertToDisplayFormat)
    {
        mTexture  = NULL;
        mImage    = image;
        mDriver   = driver;
        mName     = name;
        mAutoFree = autoFree;

        if (mDriver) {
            driver->grab();
        }

        if (convertToDisplayFormat) {
            IrrlichtImage::convertToDisplayFormat();
        }
    }

    IrrlichtImage::~IrrlichtImage()
    {
        if (mAutoFree) {
            free();
        }

        if (mDriver) {
            mDriver->drop();
        }
    }

    irr::video::ITexture* IrrlichtImage::getTexture() const
    {
        return mTexture;
    }

    int IrrlichtImage::getWidth() const
    {
        if (mTexture) {
            return mTexture->getSize().Width;
        } else if (mImage) {
            return mImage->getDimension().Width;
        }

        fcn::throwException(
            ("Trying to get the width of a non loaded image."),
            static_cast<char const *>(__FUNCTION__),
            __FILE__,
            __LINE__);
    }

    int IrrlichtImage::getHeight() const
    {
        if (mTexture) {
            return mTexture->getSize().Height;
        } else if (mImage) {
            return mImage->getDimension().Height;
        }

        fcn::throwException(
            ("Trying to get the height of a non loaded image."),
            static_cast<char const *>(__FUNCTION__),
            __FILE__,
            __LINE__);
    }

    Color IrrlichtImage::getPixel(int x, int y)
    {
        if (mImage == NULL) {
            fcn::throwException(
                ("Image has been converted to display format."),
                static_cast<char const *>(__FUNCTION__),
                __FILE__,
                __LINE__);
        }

        irr::video::SColor color = mImage->getPixel(x, y);
        return Color(color.getRed(), color.getGreen(), color.getBlue(), color.getAlpha());
    }

    void IrrlichtImage::putPixel(int x, int y, Color const & color)
    {
        if (mImage == NULL) {
            fcn::throwException(
                ("Image has been converted to display format."),
                static_cast<char const *>(__FUNCTION__),
                __FILE__,
                __LINE__);
        }

        mImage->setPixel(x, y, irr::video::SColor(color.a, color.r, color.g, color.b));
    }

    void IrrlichtImage::convertToDisplayFormat()
    {
        if (mTexture != NULL) {
            return;
        }

        if (mImage == NULL) {
            fcn::throwException(
                ("Trying to convert a non loaded image to display format."),
                static_cast<char const *>(__FUNCTION__),
                __FILE__,
                __LINE__);
        }

        bool hasPink = false;

        irr::video::SColor pink(255, 255, 0, 255);
        for (int i = 0; i < mImage->getDimension().Width; ++i) {
            for (int j = 0; j < mImage->getDimension().Height; ++j) {
                if (mImage->getPixel(i, j) == pink) {
                    hasPink = true;
                    break;
                }
            }
        }

        mTexture = mDriver->addTexture(mName.c_str(), mImage);

        if (mTexture == NULL) {
            fcn::throwException(
                ("Unable to convert image to display format!"),
                static_cast<char const *>(__FUNCTION__),
                __FILE__,
                __LINE__);
        }

        if (hasPink == true) {
            mDriver->makeColorKeyTexture(mTexture, irr::video::SColor(0, 255, 0, 255));
        }

        mImage->drop();
        mImage = NULL;
    }

    void IrrlichtImage::free()
    {
        if (mImage != NULL) {
            mImage->drop();
        }
    }
} // namespace fcn
