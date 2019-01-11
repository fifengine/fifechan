/***************************************************************************
 *   Copyright (c) 2017-2019 by the fifechan team                               *
 *   https://github.com/fifengine/fifechan                                 *
 *   This file is part of fifechan.                                        *
 *                                                                         *
 *   fifechan is free software; you can redistribute it and/or             *
 *   modify it under the terms of the GNU Lesser General Public            *
 *   License as published by the Free Software Foundation; either          *
 *   version 2.1 of the License, or (at your option) any later version.    *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *   Lesser General Public License for more details.                       *
 *                                                                         *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this library; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA          *
 ***************************************************************************/

/*      _______   __   __   __   ______   __   __   _______   __   __
 *     / _____/\ / /\ / /\ / /\ / ____/\ / /\ / /\ / ___  /\ /  |\/ /\
 *    / /\____\// / // / // / // /\___\// /_// / // /\_/ / // , |/ / /
 *   / / /__   / / // / // / // / /    / ___  / // ___  / // /| ' / /
 *  / /_// /\ / /_// / // / // /_/_   / / // / // /\_/ / // / |  / /
 * /______/ //______/ //_/ //_____/\ /_/ //_/ //_/ //_/ //_/ /|_/ /
 * \______\/ \______\/ \_\/ \_____\/ \_\/ \_\/ \_\/ \_\/ \_\/ \_\/
 *
 * Copyright (c) 2008 Mehdi Abbad a.k.a slyf0x
 *
 *
 * Per Larsson a.k.a finalman
 * Olof Naessén a.k.a jansem/yakslem
 *
 * Visit: http://guichan.sourceforge.net
 *
 * License: (BSD)
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name of Guichan nor the names of its contributors may
 *    be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "fifechan/cairo/cairoimage.hpp"

#include "fifechan/exception.hpp"

namespace fcn
{
    CairoImage::CairoImage(cairo_surface_t* surface)
    {
        mCairoSurface=surface;
    }

    CairoImage::~CairoImage()
    {
        if (mCairoSurface)
        {
            cairo_surface_destroy(mCairoSurface);
        }
    }

    int CairoImage::getWidth() const
    {
        if (!mCairoSurface)
        {
            throw FCN_EXCEPTION("Trying to get the width of a non loaded image.");
        }
        int i=cairo_image_surface_get_width(mCairoSurface);
        return i;
    }

    int CairoImage::getHeight() const
    {
        if (!mCairoSurface)
        {
            throw FCN_EXCEPTION("Trying to get the height of a non loaded image.");
        }
        int i=cairo_image_surface_get_height(mCairoSurface);
        return i;
    }

    void CairoImage::free()
    {
        if (!mCairoSurface)
        {
            throw FCN_EXCEPTION("Trying to free a non loaded image.");
        }
        cairo_surface_destroy(mCairoSurface);
        mCairoSurface=NULL;
    }


    unsigned long CairoImage::PrecomputeAlpha(const Color& color)
    {
        #warning FIXME (slyf0x#1#): Must deal with both big & little endian platforms
        double a=color.a/255.0;
        int r=(int)(a*color.r),
            g=(int)(a*color.g),
            b=(int)(a*color.b);
        return (color.a<<24 + r<<16 + g<<8 + b);
    }

    unsigned long CairoImage::GetRGB(const Color& color)
    {
        return (color.r<<16 + color.g<<8 + color.b);
    }

    Color CairoImage::GetColorFromRGB(unsigned long color)
    {
        return (Color(color>>16,(color>>8)&0x0000FF,(color&0xFF),0xFF));
    }

    Color CairoImage::GetColorFromARGB(unsigned long color)
    {
        // pixel data are stored with precomputed alpha values ex :
        // red with 50% alpha is not stored 0x80FF0000 but 0x80800000
        Color c;
        c.a=color >> 24;
        if (c.a==0)
        {
            c.b=c.r=c.g=0;
        } else
        {
            c.b=( (color & 0xFF) * 255) / c.a;
            c.g=( ( (color >> 8) & 0xFF) * 255) / c.a;
            c.r=( ( (color >> 16) & 0xFF) *255) / c.a;
        }
        return c;
    }

    Color CairoImage::getPixel(int x, int y)
    {
        if (!mCairoSurface)
        {
            throw FCN_EXCEPTION("Trying to get a pixel from a non loaded image.");
        }

        int stride=cairo_image_surface_get_stride(mCairoSurface);
        int yindex=y*stride;
        unsigned char *imagePixels=cairo_image_surface_get_data(mCairoSurface);
        if (!imagePixels)
        {
            throw FCN_EXCEPTION("Surface data are not available (surface is not of type cairo_image_surface or has been finished)");
        }
        // deal differently with each surface format
        switch(cairo_image_surface_get_format(mCairoSurface))
        {
            case CAIRO_FORMAT_ARGB32:
                return GetColorFromARGB(*((unsigned long*)(&imagePixels[x*4 + yindex])));
                break;
            case CAIRO_FORMAT_RGB24:
                return GetColorFromRGB(*((unsigned long*)(&imagePixels[x*4 + yindex])));
                break;
            case CAIRO_FORMAT_A8:
                return Color(0,0,0,imagePixels[x + yindex]);
                break;
            default :
                return Color(0,0,0);
                //do nothing
                break;
        }
    }

    void CairoImage::putPixel(int x, int y, const Color& color)
    {
        if (!mCairoSurface)
        {
            throw FCN_EXCEPTION("Trying to write a pixel on a non loaded image.");
        }

        int stride=cairo_image_surface_get_stride(mCairoSurface);
        unsigned char *imagePixels=cairo_image_surface_get_data(mCairoSurface);
        if (!imagePixels)
        {
            throw FCN_EXCEPTION("Surface data are not available (surface is not of type Image or has been finished)");
        }
        // deal differently with each surface format
        switch(cairo_image_surface_get_format(mCairoSurface))
        {
            case CAIRO_FORMAT_ARGB32:
                *((unsigned long*)(&imagePixels[x*4 + y*stride]))=PrecomputeAlpha(color);
                break;
            case CAIRO_FORMAT_RGB24:
                *((unsigned long*)(&imagePixels[x*4 + y*stride]))=GetRGB(color);
                break;
            case CAIRO_FORMAT_A8:
                imagePixels[x + y*stride]=(unsigned char)color.a;
                break;
            default :
                //do nothing
                break;
        }
    }

    cairo_surface_t* CairoImage::GetSurface()
    {
        return mCairoSurface;
    }

    void CairoImage::convertToDisplayFormat()
    {

    }

}
