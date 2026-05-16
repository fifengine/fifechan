// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Fifengine contributors

// Corresponding header include
#include "fifechan/widgets/statusbar.hpp"

// Standard library includes
#include <cstdio>
#include <memory>
#include <string>

// Project headers
#include "fifechan/font.hpp"
#include "fifechan/graphics.hpp"

namespace fcn
{

    StatusBar::StatusBar()
    {
        setHeight(24);
        setLayout(LayoutPolicy::Horizontal);

        setExpandChildren(false);

        // Create internal 3 areas:
        // 1. icon (fixed)
        // 2. left (flexible)
        // 3. right (fixed)
        mIconArea = std::make_unique<Container>();
        mIconArea->setLayout(Container::LayoutPolicy::Horizontal);
        mIconArea->setOpaque(false);
        mIconArea->setWidth(24);
        mIconArea->setHeight(24);
        add(mIconArea.get());

        mLeftArea = std::make_unique<Container>();
        mLeftArea->setLayout(Container::LayoutPolicy::Horizontal);
        mLeftArea->setOpaque(false);
        add(mLeftArea.get());

        mRightArea = std::make_unique<Container>();
        mRightArea->setLayout(Container::LayoutPolicy::Horizontal);
        mRightArea->setOpaque(false);
        mRightArea->setWidth(100);
        add(mRightArea.get());
    }

    void StatusBar::setText(std::string const & text)
    {
        mText = text;
    }

    std::string const & StatusBar::getText() const
    {
        return mText;
    }

    void StatusBar::draw(Graphics* graphics)
    {
        HorizontalBar::draw(graphics);

        // Draw border/top line
        if (isOpaque()) {
            graphics->setColor(getForegroundColor());
            graphics->drawLine(0, 0, getWidth(), 0);
        }

        std::string textToDraw = mText;

        // Fixed widths
        int const iconAreaWidth  = 24;
        int const rightAreaWidth = 100;
        int const textPadding    = 4;

        // Get text width if we have text
        int textWidth = 0;
        Font* font    = getFont();
        if (!mText.empty() && font != nullptr) {
            textWidth = font->getWidth(mText);
        }

        // Calculate minimum width needed
        // iconArea (24) + rightArea (100) + text (with padding) = 128 + textWidth
        int const minimumWidth = iconAreaWidth + rightAreaWidth + textWidth + textPadding;
        int const currentWidth = getWidth();

        // Apply shrink priority logic if enabled
        if (mShrinkPriorityEnabled && currentWidth < minimumWidth) {
            // First, calculate how much space is available for text after fixed areas
            int const availableForText = currentWidth - iconAreaWidth - rightAreaWidth - textPadding;

            // If there's still not enough space, truncate the text
            if (availableForText < textWidth && availableForText > 0) {
                if (mTruncateWithEllipsis && availableForText > 3) {
                    // Calculate how many characters fit, accounting for "..."
                    int const ellipsisWidth           = font != nullptr ? font->getWidth("...") : 15;
                    int const availableForTextContent = availableForText - ellipsisWidth;

                    if (availableForTextContent > 0) {
                        // Find characters that fit
                        int charWidth   = 0;
                        int charsToShow = 0;
                        for (size_t i = 0; i < mText.size(); ++i) {
                            charWidth += font != nullptr ? font->getWidth(std::string(1, mText.at(i))) : 8;
                            if (charWidth > availableForTextContent) {
                                break;
                            }
                            charsToShow = static_cast<int>(i) + 1;
                        }
                        textToDraw = mText.substr(0, charsToShow) + "...";
                    } else {
                        // Not even ellipsis fits, show just "..."
                        textToDraw = "...";
                    }
                } else if (!mTruncateWithEllipsis) {
                    // Truncate without ellipsis
                    int charWidth   = 0;
                    int charsToShow = 0;
                    for (size_t i = 0; i < mText.size(); ++i) {
                        charWidth += font != nullptr ? font->getWidth(std::string(1, mText.at(i))) : 8;
                        if (charWidth > availableForText) {
                            break;
                        }
                        charsToShow = static_cast<int>(i) + 1;
                    }
                    textToDraw = mText.substr(0, charsToShow);
                }
            }
            // If availableForText <= 0, text won't be drawn (no space left)
        }

        // Draw text
        if (!textToDraw.empty()) {

            // Do we have a font?
            if (font != nullptr) {
                graphics->setFont(font);

                // Vertically center text similar to other widgets
                int const textY =
                    getPaddingTop() + ((getHeight() - getPaddingTop() - getPaddingBottom() - font->getHeight()) / 2);

                // Clip text to avoid overlapping icon/right areas
                Rectangle const clipArea(
                    textPadding + iconAreaWidth,
                    getPaddingTop(),
                    getWidth() - (iconAreaWidth + rightAreaWidth + (2 * textPadding)),
                    getHeight() - 1);

                graphics->pushClipArea(clipArea);
                graphics->setColor(getForegroundColor());
                graphics->drawText(textToDraw, textPadding, textY, Graphics::Alignment::Left);
                graphics->popClipArea();
            }
            // TODO should we use a fallback font?
            // If no font is available, skip drawing text to avoid exceptions
        }
    }

    Container* StatusBar::getIconArea() const
    {
        return mIconArea.get();
    }

    Container* StatusBar::getLeftArea() const
    {
        return mLeftArea.get();
    }

    Container* StatusBar::getRightArea() const
    {
        return mRightArea.get();
    }

    void StatusBar::setShrinkPriorityEnabled(bool enabled)
    {
        mShrinkPriorityEnabled = enabled;
    }

    bool StatusBar::isShrinkPriorityEnabled() const
    {
        return mShrinkPriorityEnabled;
    }

    void StatusBar::setTruncateWithEllipsis(bool enabled)
    {
        mTruncateWithEllipsis = enabled;
    }

    bool StatusBar::isTruncateWithEllipsis() const
    {
        return mTruncateWithEllipsis;
    }
} // namespace fcn
