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
 * Copyright (c) 2004 - 2008 Olof Naess�n and Per Larsson
 *
 *
 * Per Larsson a.k.a finalman
 * Olof Naess�n a.k.a jansem/yakslem
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

/*
 * For comments regarding functions please see the header file.
 */

#include "fifechan/irrlicht/irrlichtimage.hpp"

#include "fifechan/exception.hpp"

namespace fcn
{
    IrrlichtImage::IrrlichtImage(irr::video::IImage* image,
                                 irr::video::IVideoDriver* driver,
                                 const std::string& name,
                                 bool autoFree,
                                 bool convertToDisplayFormat)
    {
        mTexture = NULL;
        mImage = image;
        mDriver = driver;
        mName = name;
        mAutoFree = autoFree;

        if (mDriver)
        {
            driver->grab();
        }

        if (convertToDisplayFormat)
        {
            IrrlichtImage::convertToDisplayFormat();
        }
    }

    IrrlichtImage::~IrrlichtImage()
    {
        if (mAutoFree)
        {
            free();
        }

        if (mDriver)
        {
            mDriver->drop();
        }
    }

    irr::video::ITexture* IrrlichtImage::getTexture() const
    {
        return mTexture;
    }

    int IrrlichtImage::getWidth() const
    {
        if (mTexture)
        {
            return mTexture->getSize().Width;
        }
        else if (mImage)
        {
            return mImage->getDimension().Width;
        }

        throw FCN_EXCEPTION("Trying to get the width of a non loaded image.");
    }

    int IrrlichtImage::getHeight() const
    {
        if (mTexture)
        {
            return mTexture->getSize().Height;
        }
        else if (mImage)
        {
            return mImage->getDimension().Height;
        }

        throw FCN_EXCEPTION("Trying to get the height of a non loaded image.");
    }

    Color IrrlichtImage::getPixel(int x, int y)
    {
        if (mImage == NULL)
        {
            throw FCN_EXCEPTION("Image has been converted to display format.");
        }

        irr::video::SColor color = mImage->getPixel(x, y);
        return Color(color.getRed(), color.getGreen(), color.getBlue(), color.getAlpha());
    }

    void IrrlichtImage::putPixel(int x, int y, const Color& color)
    {
        if (mImage == NULL)
        {
            throw FCN_EXCEPTION("Image has been converted to display format.");
        }

        mImage->setPixel(x, y, irr::video::SColor(color.a, color.r, color.g, color.b));
    }

    void IrrlichtImage::convertToDisplayFormat()
    {
        if (mTexture != NULL)
        {
            return;
        }

        if (mImage == NULL)
        {
            throw FCN_EXCEPTION("Trying to convert a non loaded image to display format.");
        }

        bool hasPink = false;

        irr::video::SColor pink(255, 255, 0, 255);
        for (int i = 0; i < mImage->getDimension().Width; ++i)
        {
            for (int j = 0; j < mImage->getDimension().Height; ++j)
            {
                if(mImage->getPixel(i, j) == pink)
                {
                    hasPink = true;
                    break;
                }
            }
        }

        mTexture = mDriver->addTexture(mName.c_str(), mImage);

        if (mTexture == NULL)
        {
            throw FCN_EXCEPTION("Unable to convert image to display format!");
        }

        if(hasPink == true)
        {
            mDriver->makeColorKeyTexture(mTexture, irr::video::SColor(0,255,0,255));
        }

        mImage->drop();
        mImage = NULL;
    }

    void IrrlichtImage::free()
    {
        if (mImage != NULL)
        {
            mImage->drop();
        }
    }
}
