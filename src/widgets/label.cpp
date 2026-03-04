// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#include "fifechan/widgets/label.hpp"

#include <string>

#include "fifechan/exception.hpp"
#include "fifechan/font.hpp"
#include "fifechan/graphics.hpp"

namespace fcn
{
    Label::Label() : mAlignment(Graphics::Alignment::Left)
    {
        adjustSize();
    }

    Label::Label(std::string const & caption) : mCaption(caption), mAlignment(Graphics::Alignment::Left)
    {
        adjustSize();
    }

    std::string const & Label::getCaption() const
    {
        return mCaption;
    }

    void Label::setCaption(std::string const & caption)
    {
        mCaption = caption;
        adjustSize();
    }

    void Label::setAlignment(Graphics::Alignment alignment)
    {
        mAlignment = alignment;
    }

    Graphics::Alignment Label::getAlignment() const
    {
        return mAlignment;
    }

    void Label::resizeToContent(bool recursion)
    {
        adjustSize();
    }

    void Label::adjustSize()
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
        int textX = 0;
        int const textY =
            offsetRec.y + getPaddingTop() +
            ((getHeight() - offsetRec.height - getPaddingTop() - getPaddingBottom() - getFont()->getHeight()) / 2);

        switch (getAlignment()) {
        case Graphics::Alignment::Left:
            textX = offsetRec.x + getPaddingLeft();
            break;
        case Graphics::Alignment::Center:
            textX = offsetRec.x + getPaddingLeft() +
                    (getWidth() - offsetRec.width - getPaddingLeft() - getPaddingRight()) / 2;
            break;
        case Graphics::Alignment::Right:
            textX = getWidth() - offsetRec.x - getPaddingRight();
            break;
        default:
            throwException("Unknown alignment.");
        }

        graphics->setFont(getFont());
        graphics->setColor(getForegroundColor());
        graphics->drawText(getCaption(), textX, textY, getAlignment());
    }
} // namespace fcn
