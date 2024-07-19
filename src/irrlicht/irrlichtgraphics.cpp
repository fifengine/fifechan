// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

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
        if (mDriver) {
            mDriver->drop();
            mDriver = 0;
        }
    }

    void IrrlichtGraphics::_beginDraw()
    {
        Rectangle area;
        area.x      = 0;
        area.y      = 0;
        area.width  = mDriver->getScreenSize().Width;
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

        if (mClipStack.empty()) {
            return;
        }
    }

    void IrrlichtGraphics::drawImage(Image const * image, int srcX, int srcY, int dstX, int dstY, int width, int height)
    {
        if (mClipStack.empty()) {
            fcn::throwException(
                "Clip stack is empty, perhaps you called a draw funtion outside of _beginDraw() and _endDraw()?",
                static_cast<char const *>(__FUNCTION__),
                __FILE__,
                __LINE__);
        }

        ClipRectangle const & top = mClipStack.top();

        IrrlichtImage const * srcImage = dynamic_cast<IrrlichtImage const *>(image);

        if (srcImage == NULL) {
            fcn::throwException(
                ("Trying to draw an image of unknown format, must be an IrrlichtImage."),
                static_cast<char const *>(__FUNCTION__),
                __FILE__,
                __LINE__);
        }

        irr::core::position2d<irr::s32> destPos(dstX + top.xOffset, dstY + top.yOffset);
        irr::core::rect<irr::s32> sourceRect(srcX, srcY, srcX + width, srcY + height);
        irr::core::rect<irr::s32> clipRect(top.x, top.y, top.x + top.width, top.y + top.height);
        irr::video::SColor color(255, 255, 255, 255);

        mDriver->draw2DImage(srcImage->getTexture(), destPos, sourceRect, &clipRect, color, true);
    }

    void IrrlichtGraphics::fillRectangle(Rectangle const & rectangle)
    {
        if (mClipStack.empty()) {
            fcn::throwException(
                "Clip stack is empty, perhaps you called a draw funtion outside of _beginDraw() and _endDraw()?",
                static_cast<char const *>(__FUNCTION__),
                __FILE__,
                __LINE__);
        }

        ClipRectangle const & top = mClipStack.top();

        Rectangle area = rectangle;
        area.x += top.xOffset;
        area.y += top.yOffset;

        if (!area.isIntersecting(top)) {
            return;
        }

        irr::video::SColor color(mColor.a, mColor.r, mColor.g, mColor.b);
        irr::core::rect<irr::s32> pos(area.x, area.y, area.x + area.width, area.y + area.height);
        irr::core::rect<irr::s32> clip(top.x, top.y, top.x + top.width, top.y + top.height);

        mDriver->draw2DRectangle(color, pos, &clip);
    }

    void IrrlichtGraphics::drawPoint(int x, int y)
    {
        if (mClipStack.empty()) {
            fcn::throwException(
                "Clip stack is empty, perhaps you called a draw funtion outside of _beginDraw() and _endDraw()?",
                static_cast<char const *>(__FUNCTION__),
                __FILE__,
                __LINE__);
        }

        ClipRectangle const & top = mClipStack.top();

        x += top.xOffset;
        y += top.yOffset;

        if (!top.isContaining(x, y))
            return;

        irr::video::SColor color(mColor.a, mColor.r, mColor.g, mColor.b);

        mDriver->drawPixel(x, y, color);
    }

    void IrrlichtGraphics::drawRectangle(Rectangle const & rectangle)
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
        if (mClipStack.empty()) {
            fcn::throwException(
                "Clip stack is empty, perhaps you called a draw funtion outside of _beginDraw() and _endDraw()?",
                static_cast<char const *>(__FUNCTION__),
                __FILE__,
                __LINE__);
        }

        ClipRectangle const & top = mClipStack.top();

        x1 += top.xOffset;
        y1 += top.yOffset;
        x2 += top.xOffset;
        y2 += top.yOffset;

        if (x2 > top.x + top.width)
            x2 = top.x + top.width;

        if (y2 > top.y + top.height)
            y2 = top.y + top.height;

        irr::core::position2d<irr::s32> start(x1, y1);
        irr::core::position2d<irr::s32> end(x2, y2);
        irr::video::SColor color(mColor.a, mColor.r, mColor.g, mColor.b);

        mDriver->draw2DLine(start, end, color);
    }

    void IrrlichtGraphics::setColor(Color const & color)
    {
        mColor = color;
    }

    Color const & IrrlichtGraphics::getColor() const
    {
        return mColor;
    }
} // namespace fcn
