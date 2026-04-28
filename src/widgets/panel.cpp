// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Fifengine contributors

// Corresponding header include
#include "fifechan/widgets/panel.hpp"

// Standard library includes
#include <string>

// Project headers (subdirs before local)
#include "fifechan/font.hpp"
#include "fifechan/graphics.hpp"

namespace fcn
{
    /**
     * Panel is a container with a title bar and optional close button.
     */
    Panel::Panel()
    {
        addMouseListener(this);
    }

    void Panel::setVisibilityState(VisibilityState state)
    {
        mVisibilityState = state;
    }

    VisibilityState Panel::getVisibilityState() const
    {
        return mVisibilityState;
    }

    void Panel::setCollapsedWidth(int width)
    {
        mCollapsedWidth = width;
    }

    int Panel::getCollapsedWidth() const
    {
        return mCollapsedWidth;
    }

    void Panel::setVisible(bool visible)
    {
        if (visible) {
            mVisibilityState = VisibilityState::Visible;
        } else {
            mVisibilityState = VisibilityState::Hidden;
        }
        Widget::setVisible(visible);
    }

    void Panel::adjustSize()
    {
        if (mVisibilityState == VisibilityState::Collapsed) {
            int width = mCollapsedWidth + (2 * getBorderSize()) + getPaddingLeft() + getPaddingRight();
            setWidth(width);
        } else if (mVisibilityState == VisibilityState::Hidden) {
            // Don't adjust size when hidden - maintain original dimensions
            // for when it becomes visible again
        } else {
            Container::adjustSize();
        }
    }

    void Panel::setTitle(std::string const & title)
    {
        mTitle = title;
    }

    std::string const & Panel::getTitle() const
    {
        return mTitle;
    }

    void Panel::setClosable(bool closable)
    {
        mClosable = closable;
    }

    bool Panel::isClosable() const
    {
        return mClosable;
    }

    void Panel::draw(Graphics* graphics)
    {
        // Handle collapsed state - draw only title bar strip without content
        if (mVisibilityState == VisibilityState::Collapsed) {
            int const titleBarHeight = getBorderSize() + getPaddingTop() + 20;

            // Draw background for collapsed title bar strip
            if (isOpaque()) {
                graphics->setColor(getBaseColor());
                graphics->fillRectangle(0, 0, getWidth(), titleBarHeight);
            }

            // Draw border
            if (getBorderSize() > 0) {
                drawBorder(graphics);
            }

            // Draw title text (clipped to collapsed width)
            if (!mTitle.empty() && getFont()) {
                graphics->setColor(getForegroundColor());
                graphics->setFont(getFont());

                int const textX = getBorderSize() + getPaddingLeft() + 4;
                int const textY = (titleBarHeight - getFont()->getHeight()) / 2;

                Rectangle const clipArea(
                    getBorderSize() + getPaddingLeft(),
                    getBorderSize() + getPaddingTop(),
                    getWidth() - (2 * getBorderSize()) - getPaddingLeft() - getPaddingRight() - 24,
                    titleBarHeight - 1);
                graphics->pushClipArea(clipArea);
                graphics->drawText(mTitle, textX, textY);
                graphics->popClipArea();
            }

            // Draw close button (if closable)
            if (mClosable) {
                int const closeX   = getWidth() - getBorderSize() - getPaddingRight() - 16;
                int const closeY   = (titleBarHeight - 12) / 2;
                mCloseButtonBounds = Rectangle(closeX, closeY, 12, 12);

                graphics->setColor(getForegroundColor());
                graphics->drawLine(
                    mCloseButtonBounds.x,
                    mCloseButtonBounds.y,
                    mCloseButtonBounds.x + mCloseButtonBounds.width,
                    mCloseButtonBounds.y + mCloseButtonBounds.height);
                graphics->drawLine(
                    mCloseButtonBounds.x,
                    mCloseButtonBounds.y + mCloseButtonBounds.height,
                    mCloseButtonBounds.x + mCloseButtonBounds.width,
                    mCloseButtonBounds.y);
            }

            return;
        }

        // Normal visible state - Container draws background and children
        Container::draw(graphics);

        if (mTitle.empty()) {
            return;
        }

        int const titleBarHeight = getBorderSize() + getPaddingTop() + 20;
        int const textX          = getBorderSize() + getPaddingLeft() + 4;
        int const textY          = (titleBarHeight - getFont()->getHeight()) / 2;

        graphics->setColor(getForegroundColor());
        graphics->setFont(getFont());

        Rectangle const clipArea(
            getBorderSize() + getPaddingLeft(),
            getBorderSize() + getPaddingTop(),
            getWidth() - (2 * getBorderSize()) - getPaddingLeft() - getPaddingRight() - 24,
            titleBarHeight - 1);
        graphics->pushClipArea(clipArea);
        graphics->drawText(mTitle, textX, textY);
        graphics->popClipArea();

        if (mClosable) {
            int const closeX   = getWidth() - getBorderSize() - getPaddingRight() - 16;
            int const closeY   = (titleBarHeight - 12) / 2;
            mCloseButtonBounds = Rectangle(closeX, closeY, 12, 12);

            graphics->setColor(getForegroundColor());
            graphics->drawLine(
                mCloseButtonBounds.x,
                mCloseButtonBounds.y,
                mCloseButtonBounds.x + mCloseButtonBounds.width,
                mCloseButtonBounds.y + mCloseButtonBounds.height);
            graphics->drawLine(
                mCloseButtonBounds.x,
                mCloseButtonBounds.y + mCloseButtonBounds.height,
                mCloseButtonBounds.x + mCloseButtonBounds.width,
                mCloseButtonBounds.y);
        }
    }

    void Panel::mousePressed(MouseEvent& event)
    {
        if (event.getSource() != this) {
            return;
        }

        if (mClosable && mCloseButtonBounds.isEmpty() == false) {
            int const x = event.getX();
            int const y = event.getY();

            if (x >= mCloseButtonBounds.x && x < mCloseButtonBounds.x + mCloseButtonBounds.width &&
                y >= mCloseButtonBounds.y && y < mCloseButtonBounds.y + mCloseButtonBounds.height) {
                setVisible(false);
                return;
            }
        }

        int const titleBarHeight = getBorderSize() + getPaddingTop() + 20;
        if (event.getY() > titleBarHeight) {
            handleContentClick(event);
        }
    }

    bool Panel::handleContentClick(MouseEvent& /*event*/)
    {
        return false;
    }
} // namespace fcn
