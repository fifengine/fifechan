// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Fifengine contributors

// Standard library includes
#include <algorithm>
#include <cassert>

// Project headers
#include "fifechan/graphics.hpp"
#include "fifechan/widgets/activitybar.hpp"
#include "fifechan/widgets/togglebutton.hpp"

namespace fcn
{
    ActivityBar::ActivityBar(int width) : mWidth(width)
    {
        setLayout(LayoutPolicy::Vertical);
        setOpaque(true);
    }

    void ActivityBar::setWidth(int width)
    {
        mWidth = width;
        // Ensure widget dimension reflects the configured width
        setSize(mWidth, getHeight());
    }

    // cppcheck-suppress duplInheritedMember
    int ActivityBar::getWidth() const
    {
        return mWidth;
    }

    void ActivityBar::setSpacing(unsigned int spacing)
    {
        mSpacing = spacing;
        setVerticalSpacing(spacing);
    }

    unsigned int ActivityBar::getSpacing() const
    {
        return mSpacing;
    }

    // cppcheck-suppress duplInheritedMember
    void ActivityBar::addActionListener(ActionListener* listener)
    {
        mActionListeners.push_back(listener);
    }

    // cppcheck-suppress constParameterPointer
    // cppcheck-suppress duplInheritedMember
    void ActivityBar::removeActionListener(ActionListener* listener)
    {
        mActionListeners.remove(listener);
    }

    void ActivityBar::showAll()
    {
        for (auto* child : getChildren()) {
            auto* toggle = dynamic_cast<ToggleButton*>(child);
            if (toggle != nullptr) {
                toggle->setSelected(true);
            }
        }
    }

    void ActivityBar::hideAll()
    {
        for (auto* child : getChildren()) {
            auto* toggle = dynamic_cast<ToggleButton*>(child);
            if (toggle != nullptr) {
                toggle->setSelected(false);
            }
        }
    }

    void ActivityBar::resizeToContent(bool recursion)
    {
        Container::resizeToContent(recursion);
        // Enforce configured width after layout
        setSize(mWidth, getHeight());
    }

    void ActivityBar::adjustSize()
    {
        Container::adjustSize();
        // Enforce configured width after adjust
        setSize(mWidth, getHeight());
    }

    void ActivityBar::draw(Graphics* graphics)
    {
        assert("graphics must not be null" && graphics != nullptr);

        // Draw background
        if (isOpaque()) {
            graphics->setColor(getBackgroundColor());
            graphics->fillRectangle(Rectangle(0, 0, getWidth(), getHeight()));

            // Draw border line on right edge
            graphics->setColor(getForegroundColor());
            graphics->drawLine(getWidth() - 1, 0, getWidth() - 1, getHeight());
        }

        // Draw children
        Container::draw(graphics);
    }

    void ActivityBar::action(ActionEvent const & event)
    {
        // Forward action events to all listeners
        for (auto* listener : mActionListeners) {
            listener->action(event);
        }
    }
} // namespace fcn
