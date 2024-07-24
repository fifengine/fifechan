// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#include "fifechan/backends/allegro/allegroimage.hpp"

#include "fifechan/exception.hpp"

namespace fcn
{
    AllegroImage::AllegroImage(BITMAP* bitmap, bool autoFree)
    {
        mAutoFree = autoFree;
        mBitmap   = bitmap;
    }

    AllegroImage::~AllegroImage()
    {
        if (mAutoFree) {
            free();
        }
    }

    BITMAP* AllegroImage::getBitmap() const
    {
        return mBitmap;
    }

    void AllegroImage::free()
    {
        if (mBitmap) {
            destroy_bitmap(mBitmap);
            mBitmap = 0;
        }
    }

    int AllegroImage::getWidth() const
    {
        if (!mBitmap) {
            fcn::throwException(
                ("Trying to get the width of a non loaded image."),
                static_cast<char const *>(__FUNCTION__),
                __FILE__,
                __LINE__);
        }

        return mBitmap->w;
    }

    int AllegroImage::getHeight() const
    {
        if (!mBitmap) {
            fcn::throwException(
                "Trying to get the height of a non loaded image.",
                static_cast<char const *>(__FUNCTION__),
                __FILE__,
                __LINE__);
        }

        return mBitmap->h;
    }

    Color AllegroImage::getPixel(int x, int y)
    {
        if (!mBitmap) {
            fcn::throwException(
                ("Trying to get a pixel from a non loaded image."),
                static_cast<char const *>(__FUNCTION__),
                __FILE__,
                __LINE__);
        }

        int c = getpixel(mBitmap, x, y);

        return Color(getr32(c), getg32(c), getb32(c), geta(32));
    }

    void AllegroImage::putPixel(int x, int y, Color const & color)
    {
        if (!mBitmap) {
            fcn::throwException(
                ("Trying to put a pixel in a non loaded image."),
                static_cast<char const *>(__FUNCTION__),
                __FILE__,
                __LINE__);
        }

        int c = makeacol_depth(32, color.r, color.g, color.b, color.a);

        putpixel(mBitmap, x, y, c);
    }

    void AllegroImage::convertToDisplayFormat()
    {
        if (!mBitmap) {
            fcn::throwException(
                "Trying to convert a non loaded image to display format.",
                static_cast<char const *>(__FUNCTION__),
                __FILE__,
                __LINE__);
        }

        BITMAP* bmp = create_bitmap(mBitmap->w, mBitmap->h);

        blit(mBitmap, bmp, 0, 0, 0, 0, bmp->w, bmp->h);

        destroy_bitmap(mBitmap);

        mBitmap = bmp;
    }
} // namespace fcn
