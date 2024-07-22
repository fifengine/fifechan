// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#include "fifechan/openlayer/openlayergraphics.hpp"

#include <OpenLayer.hpp>

#include <string>

#include "fifechan/exception.hpp"
#include "fifechan/openlayer/openlayerimage.hpp"

namespace fcn
{
    OpenLayerGraphics::OpenLayerGraphics()
    {
        setTargetPlane(640, 480);
    }

    OpenLayerGraphics::OpenLayerGraphics(int width, int height)
    {
        setTargetPlane(width, height);
    }

    OpenLayerGraphics::~OpenLayerGraphics() { }

    void OpenLayerGraphics::_beginDraw()
    {
        pushClipArea(Rectangle(0, 0, mWidth, mHeight));
    }

    void OpenLayerGraphics::_endDraw()
    {
        popClipArea();
    }

    bool OpenLayerGraphics::pushClipArea(Rectangle area)
    {
        bool result = Graphics::pushClipArea(area);

        ol::Transforms::SetPosition(mClipStack.top().xOffset, mClipStack.top().yOffset);

        ol::Canvas::SetClipping(
            mClipStack.top().x, mClipStack.top().y, mClipStack.top().width, mClipStack.top().height);

        return result;
    }

    void OpenLayerGraphics::popClipArea()
    {
        Graphics::popClipArea();

        if (mClipStack.empty()) {
            ol::Transforms::SetPosition(0, 0);
            ol::Canvas::DisableClipping();
        } else {
            ClipRectangle const top = mClipStack.top();
            ol::Transforms::SetPosition(top.xOffset, top.yOffset);
            ol::Canvas::SetClipping(top.x, top.y, top.width, top.height);
        }
    }

    void OpenLayerGraphics::setTargetPlane(int width, int height)
    {
        mWidth  = width;
        mHeight = height;
    }

    void OpenLayerGraphics::drawImage(
        Image const * image, int srcX, int srcY, int dstX, int dstY, int width, int height)
    {
        OpenLayerImage const * srcImage = dynamic_cast<OpenLayerImage const *>(image);

        if (srcImage == NULL) {
            fcn::throwException(
                "Trying to draw an image of unknown format, must be an OpenLayerImage.",
                static_cast<char const *>(__FUNCTION__),
                __FILE__,
                __LINE__);
        }

        srcImage->getBitmap()->Blit(dstX - srcX, dstY - srcY, ol::ClippedMode(srcX, srcY, width, height), 1.0f);
    }

    void OpenLayerGraphics::drawPoint(int x, int y)
    {
        ol::GfxRend::Point(x + 0.5f, y - 0.5f, mRgba);
    }

    void OpenLayerGraphics::drawLine(int x1, int y1, int x2, int y2)
    {
        mRgba.Select();
        glDisable(GL_TEXTURE_2D);
        glLineWidth(1.0f);

        glBegin(GL_LINES);
        glVertex2f(x1 + 0.5f, y1 + 0.5f);
        glVertex2f(x2 + 0.5f, y2 + 0.5f);
        glEnd();

        glBegin(GL_POINTS);
        glVertex2f(x1 + 0.5f, y1 + 0.5f);
        glEnd();

        glBegin(GL_POINTS);
        glVertex2f(x2 + 0.5f, y2 + 0.5f);
        glEnd();

        if (ol::Settings::TextureMappingUsed()) {
            glEnable(GL_TEXTURE_2D);
        }
    }

    void OpenLayerGraphics::drawRectangle(Rectangle const & rectangle)
    {
        ol::GfxRend::RectOutline(
            rectangle.x + 0.5f, rectangle.y + 0.5f, rectangle.width - 0.5f, rectangle.height - 0.5f, mRgba);
    }

    void OpenLayerGraphics::fillRectangle(Rectangle const & rectangle)
    {
        ol::GfxRend::Rect(rectangle.x, rectangle.y, rectangle.width, rectangle.height, mRgba);
    }

    void OpenLayerGraphics::setColor(Color const & color)
    {
        mColor  = color;
        mRgba.r = color.r / 255.0f;
        mRgba.g = color.g / 255.0f;
        mRgba.b = color.b / 255.0f;
        mRgba.a = color.a / 255.0f;
        mRgba.Select();
    }

    Color const & OpenLayerGraphics::getColor() const
    {
        return mColor;
    }

    ol::Rgba const & OpenLayerGraphics::getOpenLayerColor() const
    {
        return mRgba;
    }
} // namespace fcn
