/***************************************************************************
 *   Copyright (c) 2016 by the fifechan team                               *
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

#ifndef FCN_OPENGLIMAGE_HPP
#define FCN_OPENGLIMAGE_HPP

#if defined (_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#if defined (__amigaos4__)
#include <mgl/gl.h>
#elif defined (__APPLE__)
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include <string>

#include "fifechan/color.hpp"
#include "fifechan/platform.hpp"
#include "fifechan/image.hpp"

namespace fcn
{
    /**
     * OpenGL implementation of Image.
     */
    class FCN_EXTENSION_DECLSPEC OpenGLImage : public Image
    {
    public:
        /**
         * Constructor. Loads an image from an array of pixels. The pixel array is
		 * is copied in the constructor and should thus be freed after the constructor
		 * has been called.
         *
         * NOTE: The functions getPixel and putPixel are only guaranteed to work
         *       before an image has been converted to display format.
         *
         * @param pixels to load from.
         * @param width the width of the image.
         * @param height the height of the image.
         * @param convertToDisplayFormat true if the image should be converted
         *                               to display, false otherwise.
         */
        OpenGLImage(const unsigned int* pixels, int width, int height, bool convertToDisplayFormat = true);

        /**
         * Constructor. Load an image from an OpenGL texture handle. The width
		 * and height specifies the size of the "interesting" part of the
		 * texture, the real width and height of the texture are assumed to
		 * be the closest higher power of two.
         *
         * @param textureHandle the texture handle from which to load.
		 * @param width the width of the image.
		 * @param height the height of the image.
         * @param autoFree true if the surface should automatically be deleted.
         */
        OpenGLImage(GLuint textureHandle, int width, int height, bool autoFree);

        /**
         * Destructor.
         */
        virtual ~OpenGLImage();

        /**
         * Gets the OpenGL texture handle for the image.
         *
         * @return the OpenGL texture handle for the image.
         */

        virtual GLuint getTextureHandle() const;

        /**
         * Gets the width of texture.
         *
         * @return the width of the texture.
         */
        virtual int getTextureWidth() const;

        /**
         * Gets the height of the texture.
         *
         * @return the height of the texture.
         */
        virtual int getTextureHeight() const;


        // Inherited from Image

        virtual void free();

        virtual int getWidth() const;

        virtual int getHeight() const;

        virtual Color getPixel(int x, int y);

        virtual void putPixel(int x, int y, const Color& color);

        virtual void convertToDisplayFormat();

    protected:
        GLuint mTextureHandle;
        unsigned int* mPixels;
        bool mAutoFree;
        int mWidth;
        int mHeight;
		int mTextureWidth;
		int mTextureHeight;

    };
}

#endif // end FCN_OPENGLIMAGE_HPP
