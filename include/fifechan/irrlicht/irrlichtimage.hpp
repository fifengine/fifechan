/***************************************************************************
 *   Copyright (C) 2012 by the fifechan team                               *
 *   https://github.com/fifechan                                           *
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
     *
     * @author Aaron Andersen
     * @since 0.9.0
     */
    class FCN_EXTENSION_DECLSPEC IrrlichtImage : public Image
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
        IrrlichtImage(irr::video::IImage* image,
                      irr::video::IVideoDriver* driver,
                      const std::string& name,
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

        virtual void putPixel(int x, int y, const Color& color);

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
}

#endif // end FCN_IRRLICHTIMAGE_HPP
