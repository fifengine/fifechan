// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#include "fifechan/cairo/cairoimage.hpp"

#include "fifechan/exception.hpp"

namespace fcn
{
    CairoImage::CairoImage(cairo_surface_t* surface)
    {
        mCairoSurface = surface;
    }

    CairoImage::~CairoImage()
    {
        if (mCairoSurface) {
            cairo_surface_destroy(mCairoSurface);
        }
    }

    int CairoImage::getWidth() const
    {
        if (!mCairoSurface) {
            fcn::throwException(
                ("Trying to get the width of a non loaded image."),
                static_cast<char const *>(__FUNCTION__),
                __FILE__,
                __LINE__);
        }
        int i = cairo_image_surface_get_width(mCairoSurface);
        return i;
    }

    int CairoImage::getHeight() const
    {
        if (!mCairoSurface) {
            fcn::throwException(
                ("Trying to get the height of a non loaded image."),
                static_cast<char const *>(__FUNCTION__),
                __FILE__,
                __LINE__);
        }
        int i = cairo_image_surface_get_height(mCairoSurface);
        return i;
    }

    void CairoImage::free()
    {
        if (!mCairoSurface) {
            fcn::throwException(
                ("Trying to free a non loaded image."), static_cast<char const *>(__FUNCTION__), __FILE__, __LINE__);
        }
        cairo_surface_destroy(mCairoSurface);
        mCairoSurface = NULL;
    }

    uint32_t CairoImage::PrecomputeAlpha(Color const & color)
    {
        double a = color.a / 255.0;
        int r    = static_cast<int>(a * color.r);
        int g    = static_cast<int>(a * color.g);
        int b    = static_cast<int>(a * color.b);
        return (static_cast<uint32_t>(color.a) << 24) + (static_cast<uint32_t>(r) << 16) +
               (static_cast<uint32_t>(g) << 8) + static_cast<uint32_t>(b);
    }

    uint32_t CairoImage::GetRGB(Color const & color)
    {
        return (static_cast<uint32_t>(color.r) << 16) + (static_cast<uint32_t>(color.g) << 8) +
               static_cast<uint32_t>(color.b);
    }

    Color CairoImage::GetColorFromRGB(uint32_t color)
    {
        return Color(
            static_cast<uint8_t>(color >> 16),
            static_cast<uint8_t>((color >> 8) & 0xFF),
            static_cast<uint8_t>(color & 0xFF),
            0xFF);
    }

    Color CairoImage::GetColorFromARGB(uint32_t color)
    {
        Color c;
        c.a = static_cast<uint8_t>(color >> 24);
        if (c.a == 0) {
            c.b = c.r = c.g = 0;
        } else {
            c.b = static_cast<uint8_t>((color & 0xFF) * 255 / c.a);
            c.g = static_cast<uint8_t>(((color >> 8) & 0xFF) * 255 / c.a);
            c.r = static_cast<uint8_t>(((color >> 16) & 0xFF) * 255 / c.a);
        }
        return c;
    }

    Color CairoImage::getPixel(int x, int y)
    {
        if (!mCairoSurface) {
            fcn::throwException(
                ("Trying to get a pixel from a non loaded image."),
                static_cast<char const *>(__FUNCTION__),
                __FILE__,
                __LINE__);
        }

        int stride                 = cairo_image_surface_get_stride(mCairoSurface);
        int yindex                 = y * stride;
        unsigned char* imagePixels = cairo_image_surface_get_data(mCairoSurface);
        if (!imagePixels) {
            fcn::throwException(
                "Surface data are not available (surface is not of type cairo_image_surface or has been finished)",
                static_cast<char const *>(__FUNCTION__),
                __FILE__,
                __LINE__);
        }
        // deal differently with each surface format
        switch (cairo_image_surface_get_format(mCairoSurface)) {
        case CAIRO_FORMAT_ARGB32:
            return GetColorFromARGB(*reinterpret_cast<uint32_t*>(&imagePixels[x * 4 + yindex]));
            break;
        case CAIRO_FORMAT_RGB24:
            return GetColorFromRGB(*reinterpret_cast<uint32_t*>(&imagePixels[x * 4 + yindex]));
            break;
        case CAIRO_FORMAT_A8:
            return Color(0, 0, 0, imagePixels[x + yindex]);
            break;
        default:
            return Color(0, 0, 0);
            // do nothing
            break;
        }
    }

    void CairoImage::putPixel(int x, int y, Color const & color)
    {
        if (!mCairoSurface) {
            fcn::throwException(
                ("Trying to write a pixel on a non loaded image."),
                static_cast<char const *>(__FUNCTION__),
                __FILE__,
                __LINE__);
        }

        int stride                 = cairo_image_surface_get_stride(mCairoSurface);
        unsigned char* imagePixels = cairo_image_surface_get_data(mCairoSurface);
        if (!imagePixels) {
            fcn::throwException(
                ("Surface data are not available (surface is not of type Image or has been finished)"),
                static_cast<char const *>(__FUNCTION__),
                __FILE__,
                __LINE__);
        }
        // deal differently with each surface format
        switch (cairo_image_surface_get_format(mCairoSurface)) {
        case CAIRO_FORMAT_ARGB32:
            *reinterpret_cast<uint32_t*>(&imagePixels[x * 4 + y * stride]) = PrecomputeAlpha(color);
            break;
        case CAIRO_FORMAT_RGB24:
            *reinterpret_cast<uint32_t*>(&imagePixels[x * 4 + y * stride]) = GetRGB(color);
            break;
        case CAIRO_FORMAT_A8:
            imagePixels[x + y * stride] = (unsigned char)color.a;
            break;
        default:
            // do nothing
            break;
        }
    }

    cairo_surface_t* CairoImage::GetSurface()
    {
        return mCairoSurface;
    }

    void CairoImage::convertToDisplayFormat() { }

} // namespace fcn
