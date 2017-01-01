/***************************************************************************
 *   Copyright (c) 2017 by the fifechan team                               *
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

/*
 * For comments regarding functions please see the header file.
 */

#include "fifechan/irrlicht/irrlichtgraphics.hpp"

#include "fifechan/exception.hpp"
#include "fifechan/font.hpp"
#include "fifechan/image.hpp"
#include "fifechan/irrlicht/irrlichtimage.hpp"

namespace fcn
{

    IrrlichtGraphics::IrrlichtGraphics(irr::video::IVideoDriver* driver) : mDriver(driver)
    {
        if (mDriver)
            mDriver->grab();
    }

    IrrlichtGraphics::~IrrlichtGraphics()
    {
        if (mDriver)
        {
            mDriver->drop();
            mDriver = 0;
        }
    }

    void IrrlichtGraphics::_beginDraw()
    {
        Rectangle area;
        area.x = 0;
        area.y = 0;
        area.width = mDriver->getScreenSize().Width;
        area.height = mDriver->getScreenSize().Height;
        pushClipArea(area);
    }

    void IrrlichtGraphics::_endDraw()
    {
        popClipArea();
    }

    bool IrrlichtGraphics::pushClipArea(Rectangle area)
    {
        return Graphics::pushClipArea(area);
    }

    void IrrlichtGraphics::popClipArea()
    {
        Graphics::popClipArea();

        if (mClipStack.empty())
        {
            return;
        }
    }

    void IrrlichtGraphics::drawImage(const Image* image,
                                int srcX,
                                int srcY,
                                int dstX,
                                int dstY,
                                int width,
                                int height)
    {
        if (mClipStack.empty())
        {
            throw FCN_EXCEPTION("Clip stack is empty, perhaps you called a draw funtion outside of _beginDraw() and _endDraw()?");
        }

        const ClipRectangle& top = mClipStack.top();

        const IrrlichtImage* srcImage = dynamic_cast<const IrrlichtImage*>(image);

        if (srcImage == NULL)
        {
            throw FCN_EXCEPTION("Trying to draw an image of unknown format, must be an IrrlichtImage.");
        }

        irr::core::position2d<irr::s32> destPos(dstX + top.xOffset, dstY + top.yOffset);
        irr::core::rect<irr::s32> sourceRect(srcX, srcY, srcX + width, srcY + height);
        irr::core::rect<irr::s32> clipRect(top.x, top.y, top.x + top.width, top.y + top.height);
        irr::video::SColor color(255, 255, 255, 255);

        mDriver->draw2DImage(srcImage->getTexture(), destPos, sourceRect, &clipRect, color, true);
    }

    void IrrlichtGraphics::fillRectangle(const Rectangle& rectangle)
    {
        if (mClipStack.empty())
        {
            throw FCN_EXCEPTION("Clip stack is empty, perhaps you called a draw funtion outside of _beginDraw() and _endDraw()?");
        }

        const ClipRectangle& top = mClipStack.top();

        Rectangle area = rectangle;
        area.x += top.xOffset;
        area.y += top.yOffset;

        if(!area.isIntersecting(top))
        {
            return;
        }

        irr::video::SColor color(mColor.a, mColor.r, mColor.g, mColor.b);
        irr::core::rect<irr::s32> pos(area.x, area.y, area.x + area.width, area.y + area.height);
        irr::core::rect<irr::s32> clip(top.x, top.y, top.x + top.width, top.y + top.height);
        
        mDriver->draw2DRectangle(color, pos, &clip);
    }

    void IrrlichtGraphics::drawPoint(int x, int y)
    {
        if (mClipStack.empty())
        {
            throw FCN_EXCEPTION("Clip stack is empty, perhaps you called a draw funtion outside of _beginDraw() and _endDraw()?");
        }

        const ClipRectangle& top = mClipStack.top();

        x += top.xOffset;
        y += top.yOffset;

        if(!top.isContaining(x,y))
            return;

        irr::video::SColor color(mColor.a, mColor.r, mColor.g, mColor.b);

        mDriver->drawPixel(x, y, color);
    }

    void IrrlichtGraphics::drawRectangle(const Rectangle& rectangle)
    {
        int x1 = rectangle.x;
        int x2 = rectangle.x + rectangle.width - 1;
        int y1 = rectangle.y;
        int y2 = rectangle.y + rectangle.height - 1;

        drawLine(x1, y1, x2, y1);
        drawLine(x2, y1, x2, y2);
        drawLine(x1, y1, x1, y2);
        drawLine(x1, y2, x2, y2);
    }

    void IrrlichtGraphics::drawLine(int x1, int y1, int x2, int y2)
    {
        if (mClipStack.empty())
        {
            throw FCN_EXCEPTION("Clip stack is empty, perhaps you called a draw funtion outside of _beginDraw() and _endDraw()?");
        }

        const ClipRectangle& top = mClipStack.top();

        x1 += top.xOffset;
        y1 += top.yOffset;
        x2 += top.xOffset;
        y2 += top.yOffset;

        if(x2 > top.x + top.width)
            x2 = top.x + top.width;

        if(y2 > top.y + top.height)
            y2 = top.y + top.height;

        irr::core::position2d<irr::s32> start(x1, y1);
        irr::core::position2d<irr::s32> end(x2, y2);
        irr::video::SColor color(mColor.a, mColor.r, mColor.g, mColor.b);

        mDriver->draw2DLine(start, end, color);
    }

    void IrrlichtGraphics::setColor(const Color& color)
    {
        mColor = color;
    }

    const Color& IrrlichtGraphics::getColor() const
    {
        return mColor;
    }
}
