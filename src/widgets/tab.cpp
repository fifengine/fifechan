// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#include "fifechan/widgets/tab.hpp"

#include "fifechan/font.hpp"
#include "fifechan/graphics.hpp"
#include "fifechan/widgets/tabbedarea.hpp"

namespace fcn
{
    Tab::Tab() : mHasMouse(false), mTabbedArea(nullptr)
    {
        addMouseListener(this);
        setLayout(Container::LayoutPolicy::Horizontal);
        setPadding(6);
    }

    Tab::~Tab() = default;

    void Tab::adjustSize()
    {
        Container::adjustSize();
        if (mTabbedArea != nullptr) {
            mTabbedArea->adjustTabPositions();
        }
    }

    void Tab::setTabbedArea(TabbedArea* tabbedArea)
    {
        mTabbedArea = tabbedArea;
    }

    TabbedArea* Tab::getTabbedArea()
    {
        return mTabbedArea;
    }

    Rectangle Tab::getChildrenArea()
    {
        Rectangle rec;
        rec.x      = getBorderSize() + getPaddingLeft();
        rec.y      = getBorderSize() + getPaddingTop();
        rec.width  = getWidth() - 2 * getBorderSize() - getPaddingLeft() - getPaddingRight();
        rec.height = getHeight() - 2 * getBorderSize() - getPaddingTop() - getPaddingBottom();
        return rec;
    }

    void Tab::draw(Graphics* graphics)
    {
        Color const & faceColor = getBaseColor();
        int const alpha         = getBaseColor().a;
        Color highlightColor    = faceColor + 0x303030;
        highlightColor.a        = alpha;
        Color shadowColor       = faceColor - 0x303030;
        shadowColor.a           = alpha;

        Color borderColor;
        Color baseColor;

        if ((mTabbedArea != nullptr && mTabbedArea->isTabSelected(this)) || mHasMouse) {
            // Draw a border.
            graphics->setColor(highlightColor);
            graphics->drawLine(0, 0, getWidth() - 1, 0);
            graphics->drawLine(0, 1, 0, getHeight() - 1);
            graphics->setColor(shadowColor);
            graphics->drawLine(getWidth() - 1, 1, getWidth() - 1, getHeight() - 1);

            borderColor = highlightColor;
            baseColor   = getBaseColor();
        } else {
            // Draw a border.
            graphics->setColor(shadowColor);
            graphics->drawLine(0, 0, getWidth() - 1, 0);
            graphics->drawLine(0, 1, 0, getHeight() - 1);
            graphics->drawLine(getWidth() - 1, 1, getWidth() - 1, getHeight() - 1);

            baseColor   = getBaseColor();
            baseColor.a = alpha;
        }

        // Push a clip area so the other drawings don't need to worry
        // about the border.
        graphics->pushClipArea(Rectangle(1, 1, getWidth() - 2, getHeight() - 1));
        ClipRectangle const & currentClipArea = graphics->getCurrentClipArea();

        graphics->setColor(baseColor);
        graphics->fillRectangle(0, 0, currentClipArea.width, currentClipArea.height);

        if (mTabbedArea != nullptr && mTabbedArea->isFocused() && mTabbedArea->isTabSelected(this)) {
            graphics->setColor(Color(0x000000));
            graphics->drawRectangle(2, 2, currentClipArea.width - 4, currentClipArea.height - 4);
        }

        graphics->popClipArea();
    }

    void Tab::mouseEntered(MouseEvent& /*mouseEvent*/)
    {
        mHasMouse = true;
    }

    void Tab::mouseExited(MouseEvent& /*mouseEvent*/)
    {
        mHasMouse = false;
    }
} // namespace fcn
