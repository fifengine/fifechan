// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

#include "fifechan/graphics.hpp"

#include <memory>
#include <string>

#include "fifechan/cliprectangle.hpp"
#include "fifechan/exception.hpp"
#include "fifechan/font.hpp"
#include "fifechan/image.hpp"
#include "fifechan/rectangle.hpp"

namespace fcn
{
    bool Graphics::pushClipArea(Rectangle area)
    {
        // Ignore area with a negative width or height by pushing
        // an empty clip area to the stack.
        if (area.isEmpty()) {
            ClipRectangle const clip_rect;
            mClipStack.push(clip_rect);
            return true;
        }

        if (mClipStack.empty()) {
            ClipRectangle clip_rect;
            clip_rect.x       = area.x;
            clip_rect.y       = area.y;
            clip_rect.width   = area.width;
            clip_rect.height  = area.height;
            clip_rect.xOffset = area.x;
            clip_rect.yOffset = area.y;
            mClipStack.push(clip_rect);
            return true;
        }

        ClipRectangle const & top = mClipStack.top();
        ClipRectangle clip_rect;
        clip_rect         = area;
        clip_rect.xOffset = top.xOffset + clip_rect.x;
        clip_rect.yOffset = top.yOffset + clip_rect.y;
        clip_rect.x += top.xOffset;
        clip_rect.y += top.yOffset;

        clip_rect = top.intersection(clip_rect);

        mClipStack.push(clip_rect);

        return !clip_rect.isEmpty();
    }

    void Graphics::popClipArea()
    {

        if (mClipStack.empty()) {
            throwException("Tried to pop clip area from empty stack.");
        }

        mClipStack.pop();
    }

    ClipRectangle const & Graphics::getCurrentClipArea()
    {
        if (mClipStack.empty()) {
            throwException("The clip area stack is empty.");
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

    std::shared_ptr<Font> Graphics::createFont(std::string const & filename, int size)
    {
        (void)filename;
        (void)size;
        return nullptr;
    }

    void Graphics::drawText(std::string const & text, int x, int y, Alignment alignment)
    {
        if (mFont == nullptr) {
            throwException("No font set.");
        }

        switch (alignment) {
        case Alignment::Left:
            mFont->drawString(this, text, x, y);
            break;
        case Alignment::Center:
            mFont->drawString(this, text, x - (mFont->getWidth(text) / 2), y);
            break;
        case Alignment::Right:
            mFont->drawString(this, text, x - mFont->getWidth(text), y);
            break;
        default:
            throwException("Unknown alignment.");
        }
    }
} // namespace fcn
