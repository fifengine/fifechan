// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#include "fifechan/openlayer/openlayerimage.hpp"

#include "fifechan/exception.hpp"

namespace fcn
{
    OpenLayerImage::OpenLayerImage(BITMAP* allegroBitmap, bool autoFree) :
        mOpenLayerBitmap(NULL), mAllegroBitmap(allegroBitmap), mAutoFree(autoFree)

    {
        if (mAllegroBitmap == NULL) {
            fcn::throwException(
                (std::string("Unable to load bitmap.")), static_cast<char const *>(__FUNCTION__), __FILE__, __LINE__);
        }
    }

    OpenLayerImage::~OpenLayerImage()
    {
        if (mAutoFree) {
            free();
        }
    }

    ol::Bitmap* OpenLayerImage::getBitmap() const
    {
        return mOpenLayerBitmap;
    }

    void OpenLayerImage::free()
    {
        if (mOpenLayerBitmap != NULL) {
            delete mOpenLayerBitmap;
            mOpenLayerBitmap = NULL;
        }

        if (mAllegroBitmap != NULL) {
            destroy_bitmap(mAllegroBitmap);
            mAllegroBitmap = NULL;
        }
    }

    int OpenLayerImage::getWidth() const
    {
        if (mAllegroBitmap == NULL && mOpenLayerBitmap == NULL) {
            fcn::throwException(
                ("Trying to get the width of a non loaded image."),
                static_cast<char const *>(__FUNCTION__),
                __FILE__,
                __LINE__);
        }

        if (mOpenLayerBitmap == NULL) {
            return mAllegroBitmap->w;
        }

        return mOpenLayerBitmap->Width();
    }

    int OpenLayerImage::getHeight() const
    {
        if (mAllegroBitmap == NULL && mOpenLayerBitmap == NULL) {
            fcn::throwException(
                ("Trying to get the height of a non loaded image."),
                static_cast<char const *>(__FUNCTION__),
                __FILE__,
                __LINE__);
        }

        if (mOpenLayerBitmap == NULL) {
            return mAllegroBitmap->h;
        }

        return mOpenLayerBitmap->Height();
    }

    Color OpenLayerImage::getPixel(int x, int y)
    {
        if (mAllegroBitmap == NULL && mOpenLayerBitmap == NULL) {
            fcn::throwException(
                ("Trying to get a pixel from a non loaded image."),
                static_cast<char const *>(__FUNCTION__),
                __FILE__,
                __LINE__);
        }

        if (mOpenLayerBitmap == NULL) {
            int color = getpixel(mAllegroBitmap, x, y);
            return Color(getr(color), getg(color), getb(color), 255);
        }

        return Color(mOpenLayerBitmap->GetPixelPacked(x, y));
    }

    void OpenLayerImage::putPixel(int x, int y, Color const & color)
    {
        if (mAllegroBitmap == NULL && mOpenLayerBitmap == NULL) {
            fcn::throwException(
                ("Trying to put a pixel in a non loaded image."),
                static_cast<char const *>(__FUNCTION__),
                __FILE__,
                __LINE__);
        }

        if (mAllegroBitmap == NULL) {
            return;
        }

        int c = makeacol_depth(32, color.r, color.g, color.b, color.a);

        putpixel(mAllegroBitmap, x, y, c);
    }

    void OpenLayerImage::convertToDisplayFormat()
    {
        if (mAllegroBitmap == NULL) {
            fcn::throwException(
                ("Trying to convert a non loaded image to display format."),
                static_cast<char const *>(__FUNCTION__),
                __FILE__,
                __LINE__);
        }

        if (mOpenLayerBitmap != NULL) {
            fcn::throwException(
                ("Trying to convert an image to display format which has already been converted."),
                static_cast<char const *>(__FUNCTION__),
                __FILE__,
                __LINE__);
        }

        mOpenLayerBitmap = new ol::Bitmap(mAllegroBitmap, false, true);
        mOpenLayerBitmap->SendToGPU();

        destroy_bitmap(mAllegroBitmap);
        mAllegroBitmap = NULL;
    }
} // namespace fcn
