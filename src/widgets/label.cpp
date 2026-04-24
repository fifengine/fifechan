// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

// Corresponding header include
#include "fifechan/widgets/label.hpp"

// Standard library includes
#include <string>
#include <utility>

// Project headers (subdirs before local)
#include "fifechan/exception.hpp"
#include "fifechan/font.hpp"
#include "fifechan/graphics.hpp"

namespace fcn
{
    Label::Label()
    {
        adjustSizeImpl();
    }

    Label::Label(std::string caption) : mCaption(std::move(caption))
    {
        adjustSizeImpl();
    }

    std::string const & Label::getCaption() const
    {
        return mCaption;
    }

    void Label::setCaption(std::string const & caption)
    {
        mCaption = caption;
        adjustSizeImpl();
    }

    void Label::setAlignment(Graphics::Alignment alignment)
    {
        mAlignment = alignment;
    }

    Graphics::Alignment Label::getAlignment() const
    {
        return mAlignment;
    }

    void Label::setVerticalAlignment(Graphics::VerticalAlignment alignment)
    {
        mVerticalAlignment = alignment;
    }

    Graphics::VerticalAlignment Label::getVerticalAlignment() const
    {
        return mVerticalAlignment;
    }

    void Label::resizeToContent(bool recursion)
    {
        (void)recursion; // unused parameter

        adjustSizeImpl();
    }

    void Label::adjustSize()
    {
        adjustSizeImpl();
    }

    void Label::adjustSizeImpl()
    {
        setSize(
            (2 * getBorderSize()) + getPaddingLeft() + getPaddingRight() + getFont()->getWidth(mCaption),
            (2 * getBorderSize()) + getPaddingTop() + getPaddingBottom() + getFont()->getHeight());
    }

    void Label::draw(Graphics* graphics)
    {
        // draw border or frame
        if (getBorderSize() > 0) {
            if (isFocused() && (getSelectionMode() & Widget::SelectionMode::Border) == Widget::SelectionMode::Border) {
                drawSelectionFrame(graphics);
            } else {
                drawBorder(graphics);
            }
        }

        Rectangle const offsetRec(getBorderSize(), getBorderSize(), 2 * getBorderSize(), 2 * getBorderSize());

        int const contentLeft   = offsetRec.x + getPaddingLeft();
        int const contentRight  = getWidth() - offsetRec.x - getPaddingRight();
        int const contentTop    = offsetRec.y + getPaddingTop();
        int const contentBottom = getHeight() - offsetRec.y - getPaddingBottom();

        int const contentWidth  = contentRight - contentLeft;
        int const contentHeight = contentBottom - contentTop;

        int const fontHeight = getFont()->getHeight();

        int textX = 0;
        int textY = 0;

        // Vertical alignment
        switch (mVerticalAlignment) {
        case Graphics::VerticalAlignment::Top:
            textY = contentTop;
            break;
        case Graphics::VerticalAlignment::Center:
            textY = contentTop + (contentHeight - fontHeight) / 2;
            break;
        case Graphics::VerticalAlignment::Bottom:
            textY = contentBottom - fontHeight;
            break;
        default:
            throwException("Unknown vertical alignment.");
        }

        // Horizontal alignment
        switch (getAlignment()) {
        case Graphics::Alignment::Left:
            textX = contentLeft;
            break;
        case Graphics::Alignment::Center:
            textX = contentLeft + contentWidth / 2;
            break;
        case Graphics::Alignment::Right:
            textX = contentRight;
            break;
        default:
            throwException("Unknown alignment.");
        }

        graphics->setFont(getFont());
        graphics->setColor(getForegroundColor());
        graphics->drawText(getCaption(), textX, textY, getAlignment());
    }
} // namespace fcn
