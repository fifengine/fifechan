// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#include "fifechan/hge/hgegraphics.hpp"

#include "fifechan/cliprectangle.hpp"
#include "fifechan/exception.hpp"
#include "fifechan/hge/hgeimage.hpp"
#include "fifechan/rectangle.hpp"

namespace fcn
{
    HGE* HGEGraphics::mHGE = NULL;

    HGEGraphics::HGEGraphics() : mClipNull(false)
    {
        mHGE = hgeCreate(HGE_VERSION);

        mHardwareColor = 0;
    }

    HGEGraphics::~HGEGraphics()
    {
        mHGE->Release();
    }

    void HGEGraphics::_beginDraw()
    {
        pushClipArea(Rectangle(0, 0, mHGE->System_GetState(HGE_SCREENWIDTH), mHGE->System_GetState(HGE_SCREENHEIGHT)));
    }

    void HGEGraphics::_endDraw()
    {
        // pop the clip area pushed in _beginDraw
        popClipArea();
    }

    bool HGEGraphics::pushClipArea(Rectangle area)
    {
        bool result = Graphics::pushClipArea(area);

        ClipRectangle const top = mClipStack.top();

        // HGE won't let you set clip areas
        // that have zero width or height
        // so we have to check for that.
        if (top.width == 0 || top.height == 0) {
            mClipNull = true;
        } else {
            mClipNull = false;
            mHGE->Gfx_SetTransform(top.xOffset, top.yOffset);
            mHGE->Gfx_SetClipping(top.x, top.y, top.width, top.height);
        }
        return result;
    }

    void HGEGraphics::popClipArea()
    {
        Graphics::popClipArea();

        if (mClipStack.empty()) {
            mHGE->Gfx_SetClipping();

            return;
        } else {
            ClipRectangle const top = mClipStack.top();

            // HGE won't let you set clip areas
            // that have zero width or height
            // so we have to check for that.
            if (top.width == 0 || top.height == 0) {
                mClipNull = true;
            } else {
                mClipNull = false;
                mHGE->Gfx_SetTransform(top.xOffset, top.yOffset);
                mHGE->Gfx_SetClipping(top.x, top.y, top.width, top.height);
            }
        }
    }

    void HGEGraphics::drawImage(Image const * image, int srcX, int srcY, int dstX, int dstY, int width, int height)
    {
        if (mClipNull) {
            return;
        }

        HGEImage const * hgeImage = static_cast<HGEImage const *>(image);

        if (hgeImage == NULL) {
            fcn::throwException(
                ("Trying to draw an image of unknown format, must be an HGEImage."),
                static_cast<char const *>(__FUNCTION__),
                __FILE__,
                __LINE__);
        }

        ClipRectangle const top = mClipStack.top();

        dstX += top.xOffset;
        dstY += top.yOffset;

        hgeImage->getSprite()->SetTextureRect(srcX, srcY, width, height);
        hgeImage->getSprite()->Render(dstX, dstY);
    }

    void HGEGraphics::drawImage(Image const * image, int dstX, int dstY)
    {
        if (mClipNull) {
            return;
        }

        drawImage(image, 0, 0, dstX, dstY, image->getWidth(), image->getHeight());
    }

    void HGEGraphics::drawPoint(int x, int y)
    {
        if (mClipNull) {
            return;
        }

        ClipRectangle const top = mClipStack.top();

        x += top.xOffset;
        y += top.yOffset;

        mHGE->Gfx_RenderLine(x, y, x + 1, y, mHardwareColor);
    }

    void HGEGraphics::drawLine(int x1, int y1, int x2, int y2)
    {
        if (mClipNull) {
            return;
        }

        // As HGE omits the last pixel we need to adjust the coordinates
        // before drawing the line. If it's a vertical or horizontal line
        // all we have to do is add the omitted pixel.
        if (y1 == y2 || x1 == x2) {
            x2++;
            y2++;
        }
        // If it's not a vertical or horizontal line it gets a little bit
        // trickier.
        else {
            // If y2 is greater than y1 then we know y2 will be omitted as
            // it will be a part of the last pixel coordinate.
            if (y2 > y1) {
                y2++;
            }
            // Else will y1 be omitted.
            else {
                y1++;
            }
            // The same thing applies for the x coordinates. If x2 is greater
            // than x1 then we know x2 will be omitted as it will be a part of
            // the last pixel coordinate.
            if (x2 > x1) {
                x2++;
            }
            // Else will x1 be omitted.
            else {
                x1++;
            }
        }

        ClipRectangle const top = mClipStack.top();

        x1 += top.xOffset;
        y1 += top.yOffset;
        x2 += top.xOffset;
        y2 += top.yOffset;

        mHGE->Gfx_RenderLine(x1, y1, x2, y2, mHardwareColor);
    }

    void HGEGraphics::drawRectangle(Rectangle const & rectangle)
    {
        if (mClipNull) {
            return;
        }

        int x1 = rectangle.x;
        int y1 = rectangle.y;
        int x2 = rectangle.x + rectangle.width;
        int y2 = rectangle.y + rectangle.height;

        ClipRectangle const top = mClipStack.top();

        x1 += top.xOffset;
        y1 += top.yOffset;
        x2 += top.xOffset;
        y2 += top.yOffset;

        mHGE->Gfx_RenderLine(x1, y1 + 1, x2, y1, mHardwareColor);
        mHGE->Gfx_RenderLine(x2, y1 + 1, x2, y2 - 1, mHardwareColor);
        mHGE->Gfx_RenderLine(x2, y2, x1 + 1, y2, mHardwareColor);
        mHGE->Gfx_RenderLine(x1 + 1, y2, x1 + 1, y1 + 1, mHardwareColor);
    }

    void HGEGraphics::fillRectangle(Rectangle const & rectangle)
    {
        if (mClipNull) {
            return;
        }

        // We need to compensate for the fact that HGE doesn't
        // seem to include the the coordinate pixels when rendering
        // a quad.
        int x1 = rectangle.x;
        int y1 = rectangle.y;
        int x2 = rectangle.x + rectangle.width;
        int y2 = rectangle.y + rectangle.height;

        ClipRectangle const top = mClipStack.top();

        x1 += top.xOffset;
        y1 += top.yOffset;
        x2 += top.xOffset;
        y2 += top.yOffset;

        hgeQuad quad;

        quad.tex = NULL;

        quad.v[0].x   = x1;
        quad.v[0].y   = y1;
        quad.v[0].col = mHardwareColor;

        quad.v[1].x   = x2;
        quad.v[1].y   = y1;
        quad.v[1].col = mHardwareColor;

        quad.v[2].x   = x2;
        quad.v[2].y   = y2;
        quad.v[2].col = mHardwareColor;

        quad.v[3].x   = x1;
        quad.v[3].y   = y2;
        quad.v[3].col = mHardwareColor;

        int i;
        for (i = 0; i < 4; ++i) {
            quad.v[i].z = 0.5f;
        }

        quad.blend = BLEND_DEFAULT;

        mHGE->Gfx_RenderQuad(&quad);
    }

    void HGEGraphics::setColor(Color const & color)
    {
        mColor = color;

        mHardwareColor = ARGB(color.a, color.r, color.g, color.b);
    }

    Color const & HGEGraphics::getColor() const
    {
        return mColor;
    }
} // namespace fcn
