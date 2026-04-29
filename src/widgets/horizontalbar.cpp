// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Fifengine contributors

// Standard library includes
#include <algorithm>

// Project headers
#include "fifechan/widgets/horizontalbar.hpp"

namespace fcn
{
    HorizontalBar::HorizontalBar()
    {
        setLayout(LayoutPolicy::Horizontal);
        setOpaque(true);
    }

    void HorizontalBar::setSpacing(unsigned int spacing)
    {
        mSpacing = spacing;
        setHorizontalSpacing(spacing);
    }

    unsigned int HorizontalBar::getSpacing() const
    {
        return mSpacing;
    }

    // cppcheck-suppress duplInheritedMember
    void HorizontalBar::setPadding(unsigned int padding)
    {
        mPadding = padding;
    }

    unsigned int HorizontalBar::getPadding() const
    {
        return mPadding;
    }

    void HorizontalBar::setFixedHeight(unsigned int height)
    {
        mFixedHeight = height;
    }

    unsigned int HorizontalBar::getFixedHeight() const
    {
        return mFixedHeight;
    }

    void HorizontalBar::setClipping(bool clip)
    {
        mClipping = clip;
    }

    bool HorizontalBar::isClipping() const
    {
        return mClipping;
    }

    void HorizontalBar::setExpandChildren(bool expand)
    {
        mExpandChildren = expand;
    }

    bool HorizontalBar::isExpandChildren() const
    {
        return mExpandChildren;
    }

    void HorizontalBar::resizeToContent(bool recursion)
    {
        // Disable all expansion to prevent equal sizing
        bool const savedHExpand = isHorizontalExpand();
        setHorizontalExpand(false);

        bool const savedUniform = isUniformSize();
        setUniformSize(false);

        Container::resizeToContent(recursion);

        // Post-process: shrink bar to fit content exactly (don't expand to container width)
        if (!mExpandChildren) {
            int contentW = 0;
            for (auto const * child : getChildren()) {
                if (child == nullptr || !child->isVisible()) {
                    continue;
                }
                int const childEnd = child->getX() + child->getWidth() + child->getMarginRight();
                contentW           = std::max(contentW, childEnd);
            }
            contentW += getPaddingLeft() + getPaddingRight();
            if (contentW > 0 && contentW < getWidth()) {
                setWidth(contentW);
            }
        }

        setHorizontalExpand(savedHExpand);
        setUniformSize(savedUniform);

        // Apply fixed height if set
        if (mFixedHeight > 0) {
            setHeight(mFixedHeight);
        }
    }

    void HorizontalBar::adjustSize()
    {
        // Disable all expansion to prevent equal sizing
        bool const savedHExpand = isHorizontalExpand();
        setHorizontalExpand(false);

        bool const savedUniform = isUniformSize();
        setUniformSize(false);

        Container::adjustSize();

        // Post-process: shrink bar to fit content exactly
        if (!mExpandChildren) {
            int contentW = 0;
            for (auto const * child : getChildren()) {
                if (child == nullptr || !child->isVisible()) {
                    continue;
                }
                int const childEnd = child->getX() + child->getWidth() + child->getMarginRight();
                contentW           = std::max(contentW, childEnd);
            }
            contentW += getPaddingLeft() + getPaddingRight();
            if (contentW > 0 && contentW < getWidth()) {
                setWidth(contentW);
            }
        }

        setHorizontalExpand(savedHExpand);
        setUniformSize(savedUniform);

        // Apply fixed height if set
        if (mFixedHeight > 0) {
            setHeight(mFixedHeight);
        }
    }

    void HorizontalBar::draw(Graphics* graphics)
    {
        // Draw background
        if (isOpaque()) {
            graphics->setColor(getBackgroundColor());
            graphics->fillRectangle(Rectangle(0, 0, getWidth(), getHeight()));

            // Draw 1px border at bottom
            graphics->setColor(getForegroundColor());
            graphics->drawLine(0, getHeight() - 1, getWidth(), getHeight() - 1);
        }

        // Draw children
        Container::draw(graphics);
    }
} // namespace fcn
