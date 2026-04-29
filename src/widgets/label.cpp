// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

// Corresponding header include
#include "fifechan/widgets/label.hpp"

// Standard library includes
#include <algorithm>
#include <numeric>
#include <string>
#include <utility>
#include <vector>

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
        // Support multi-line captions by measuring each line separately.
        std::vector<std::string> lines;
        {
            std::string::size_type start = 0;
            while (start <= mCaption.size()) {
                auto pos = mCaption.find('\n', start);
                if (pos == std::string::npos) {
                    lines.push_back(mCaption.substr(start));
                    break;
                }
                lines.push_back(mCaption.substr(start, pos - start));
                start = pos + 1;
            }
        }

        int maxWidth = std::accumulate(lines.begin(), lines.end(), 0, [&](int acc, std::string const & line) {
            return std::max(acc, getFont()->getWidth(line));
        });

        int const lineCount   = static_cast<int>(lines.size());
        int const totalHeight = lineCount * getFont()->getHeight();

        setSize(
            (2 * getBorderSize()) + getPaddingLeft() + getPaddingRight() + maxWidth,
            (2 * getBorderSize()) + getPaddingTop() + getPaddingBottom() + totalHeight);
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

        // Count lines to compute total text block height for proper vertical alignment
        int lineCount             = static_cast<int>(std::count(getCaption().begin(), getCaption().end(), '\n')) + 1;
        int const totalTextHeight = lineCount * fontHeight;

        int textX = 0;
        int textY = 0;

        // Vertical alignment
        switch (mVerticalAlignment) {
        case Graphics::VerticalAlignment::Top:
            textY = contentTop;
            break;
        case Graphics::VerticalAlignment::Center:
            textY = contentTop + (contentHeight - totalTextHeight) / 2;
            break;
        case Graphics::VerticalAlignment::Bottom:
            textY = contentBottom - totalTextHeight;
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

        // Draw multi-line caption by splitting on '\n' and drawing each line with font height spacing.
        std::string::size_type start = 0;
        int lineIndex                = 0;
        int const fontHeightLocal    = getFont()->getHeight();
        while (start <= getCaption().size()) {
            auto pos = getCaption().find('\n', start);
            std::string line;
            if (pos == std::string::npos) {
                line  = getCaption().substr(start);
                start = getCaption().size() + 1; // exit
            } else {
                line  = getCaption().substr(start, pos - start);
                start = pos + 1;
            }

            int const y = textY + (lineIndex * fontHeightLocal);

            // For center/right alignment, Graphics::drawText uses alignment to offset x accordingly.
            graphics->drawText(line, textX, y, getAlignment());

            ++lineIndex;
        }
    }
} // namespace fcn
