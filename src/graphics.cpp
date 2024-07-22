// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#include "fifechan/graphics.hpp"

#include <string>

#include "fifechan/cliprectangle.hpp"
#include "fifechan/exception.hpp"
#include "fifechan/font.hpp"
#include "fifechan/image.hpp"
#include "fifechan/rectangle.hpp"

namespace fcn
{
    Graphics::Graphics() = default;

    bool Graphics::pushClipArea(Rectangle area)
    {
        // Ignore area with a negate width or height
        // by simple pushing an empty clip area
        // to the stack.
        if (area.isEmpty()) {
            ClipRectangle const carea;
            mClipStack.push(carea);
            return true;
        }

        if (mClipStack.empty()) {
            ClipRectangle carea;
            carea.x       = area.x;
            carea.y       = area.y;
            carea.width   = area.width;
            carea.height  = area.height;
            carea.xOffset = area.x;
            carea.yOffset = area.y;
            mClipStack.push(carea);
            return true;
        }

        ClipRectangle const & top = mClipStack.top();
        ClipRectangle carea;
        carea         = area;
        carea.xOffset = top.xOffset + carea.x;
        carea.yOffset = top.yOffset + carea.y;
        carea.x += top.xOffset;
        carea.y += top.yOffset;

        carea = top.intersection(carea);

        mClipStack.push(carea);

        return !carea.isEmpty();
    }

    void Graphics::popClipArea()
    {

        if (mClipStack.empty()) {
            fcn::throwException(
                "Tried to pop clip area from empty stack.",
                static_cast<char const *>(__FUNCTION__),
                __FILE__,
                __LINE__);
        }

        mClipStack.pop();
    }

    ClipRectangle const & Graphics::getCurrentClipArea()
    {
        if (mClipStack.empty()) {
            fcn::throwException(
                "The clip area stack is empty.", static_cast<char const *>(__FUNCTION__), __FILE__, __LINE__);
        }

        return mClipStack.top();
    }

    void Graphics::drawImage(Image const * image, int dstX, int dstY)
    {
        drawImage(image, 0, 0, dstX, dstY, image->getWidth(), image->getHeight());
    }

    void Graphics::setFont(Font* font)
    {
        mFont = font;
    }

    void Graphics::drawText(std::string const & text, int x, int y, Alignment alignment)
    {
        if (mFont == nullptr) {
            fcn::throwException("No font set.", static_cast<char const *>(__FUNCTION__), __FILE__, __LINE__);
        }

        switch (alignment) {
        case Left:
            mFont->drawString(this, text, x, y);
            break;
        case Center:
            mFont->drawString(this, text, x - mFont->getWidth(text) / 2, y);
            break;
        case Right:
            mFont->drawString(this, text, x - mFont->getWidth(text), y);
            break;
        default:
            fcn::throwException("Unknown alignment.", static_cast<char const *>(__FUNCTION__), __FILE__, __LINE__);
        }
    }
} // namespace fcn
