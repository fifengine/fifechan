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
 * Copyright (c) 2004 - 2008 Olof Naessén and Per Larsson
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

#ifndef FCN_OPENGLALLEGROIMAGELOADER_HPP
#define FCN_OPENGLALLEGROIMAGELOADER_HPP

#include <allegro.h>
#include <alleggl.h>

#include <fifechan/imageloader.hpp>
#include <fifechan/exception.hpp>
#include <fifechan/opengl/openglimage.hpp>

namespace fcn
{
    /**
     * OpenGL ImageLoader that loads images with Allegro.
     */
    class OpenGLAllegroImageLoader : public ImageLoader
    {
    public:
        
        // Inherited from ImageLoader

        virtual Image* load(const std::string& filename,
                            bool convertToDisplayFormat = true)
       
        {
            BITMAP *bmp = load_bitmap(filename.c_str(), NULL);
            
            if (bmp == NULL)
            {
                throw FCN_EXCEPTION(
                        std::string("Unable to load image file: ") + filename);
            }

            int bitmapBpp = bitmap_color_depth(bmp);
            
            // As a BITMAP without an alpha channel in Allegro has
            // all it's alpha values set to zero we have to check
            // if the BITMAP has an alpha channel. If no alpha channel
            // exists then we need to change the alpha values to 255
            // before creating the OpenGL image.
            bool hasAlphaChannel = false;
            int x, y;
            for (y = 0; y < bmp->h; y++)
            {
                for (x = 0; x < bmp->w; x++)
                {
                    int pixel = getpixel(bmp, x, y);
                    if (geta_depth(bitmapBpp, pixel) != 0)
                    {                        
                        hasAlphaChannel = true;                
                    }
                }
            }

            unsigned int *pixels = new unsigned int[bmp->w * bmp->h];
            
            for (y = 0; y < bmp->h; y++)
            {
                for (x = 0; x < bmp->w; x++)
                {
                    int pixel = getpixel(bmp,x, y);

                    if (!hasAlphaChannel)
                    {                        
                        pixels[x + y * bmp->w] = pixel | 0xff000000;
                    }
                    else
                    {
                        pixels[x + y * bmp->w] = pixel;
                    }
                }
            }

            OpenGLImage *image = new OpenGLImage(pixels,
                                                 bmp->w,
                                                 bmp->h,
                                                 convertToDisplayFormat);

            delete[] pixels;
            destroy_bitmap(bmp);

            return image;
        }
    };
}

#endif // end FCN_OPENGLALLEGROIMAGELOADER_HPP
