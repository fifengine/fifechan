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

    unsigned long CairoImage::PrecomputeAlpha(Color const & color)
    {
#warning FIXME (slyf0x#1#): Must deal with both big & little endian platforms
        double a = color.a / 255.0;
        int r = (int)(a * color.r), g = (int)(a * color.g), b = (int)(a * color.b);
        return (color.a << 24 + r << 16 + g << 8 + b);
    }

    unsigned long CairoImage::GetRGB(Color const & color)
    {
        return (color.r << 16 + color.g << 8 + color.b);
    }

    Color CairoImage::GetColorFromRGB(unsigned long color)
    {
        return (Color(color >> 16, (color >> 8) & 0x0000FF, (color & 0xFF), 0xFF));
    }

    Color CairoImage::GetColorFromARGB(unsigned long color)
    {
        // pixel data are stored with precomputed alpha values ex :
        // red with 50% alpha is not stored 0x80FF0000 but 0x80800000
        Color c;
        c.a = color >> 24;
        if (c.a == 0) {
            c.b = c.r = c.g = 0;
        } else {
            c.b = ((color & 0xFF) * 255) / c.a;
            c.g = (((color >> 8) & 0xFF) * 255) / c.a;
            c.r = (((color >> 16) & 0xFF) * 255) / c.a;
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
            return GetColorFromARGB(*((unsigned long*)(&imagePixels[x * 4 + yindex])));
            break;
        case CAIRO_FORMAT_RGB24:
            return GetColorFromRGB(*((unsigned long*)(&imagePixels[x * 4 + yindex])));
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
            *((unsigned long*)(&imagePixels[x * 4 + y * stride])) = PrecomputeAlpha(color);
            break;
        case CAIRO_FORMAT_RGB24:
            *((unsigned long*)(&imagePixels[x * 4 + y * stride])) = GetRGB(color);
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
