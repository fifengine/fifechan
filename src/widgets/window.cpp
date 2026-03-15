// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#include "fifechan/widgets/window.hpp"

#include <algorithm>
#include <string>

#include "fifechan/exception.hpp"
#include "fifechan/font.hpp"
#include "fifechan/graphics.hpp"
#include "fifechan/mouseinput.hpp"

namespace fcn
{
    Window::Window()
    {
        setBorderSize(1);
        setPadding(2);

        addMouseListener(this);
    }

    Window::Window(std::string const & caption)
    {
        setCaption(caption);
        setBorderSize(1);
        setPadding(2);

        addMouseListener(this);
    }

    Window::~Window() = default;

    void Window::setTitleBarHeight(unsigned int height)
    {
        mTitleBarHeight = height;
    }

    unsigned int Window::getTitleBarHeight() const
    {
        return mTitleBarHeight;
    }

    void Window::setInnerBorderSize(unsigned int border)
    {
        mInnerBorderSize = border;
    }

    unsigned int Window::getInnerBorderSize() const
    {
        return mInnerBorderSize;
    }

    void Window::setCaption(std::string const & caption)
    {
        mCaption = caption;
    }

    std::string const & Window::getCaption() const
    {
        return mCaption;
    }

    void Window::setAlignment(Graphics::Alignment alignment)
    {
        mAlignment = alignment;
    }

    Graphics::Alignment Window::getAlignment() const
    {
        return mAlignment;
    }

    void Window::drawInnerBorder(Graphics* graphics)
    {
        Color const & faceColor = getBaseColor();
        Color highlightColor;
        Color shadowColor;
        int const alpha  = getBaseColor().a;
        highlightColor   = faceColor + 0x303030;
        highlightColor.a = alpha;
        shadowColor      = faceColor - 0x303030;
        shadowColor.a    = alpha;

        int const x      = getBorderSize() + getPaddingLeft();
        int const y      = getBorderSize() + getPaddingTop() + getTitleBarHeight();
        int const width  = getWidth() - getBorderSize() - getPaddingRight() - 1;
        int const height = getHeight() - getBorderSize() - getPaddingBottom() - 1;

        unsigned int i = 0;
        for (i = 0; i < getInnerBorderSize(); ++i) {
            graphics->setColor(shadowColor);
            graphics->drawLine(x + i, y + i, width - i, y + i);
            graphics->drawLine(x + i, y + i + 1, x + i, height - i - 1);
            graphics->setColor(highlightColor);
            graphics->drawLine(width - i, y + i + 1, width - i, height - i);
            graphics->drawLine(x + i, height - i, width - i - 1, height - i);
        }
    }

    void Window::draw(Graphics* graphics)
    {
        Color const & faceColor = getBaseColor();
        Color highlightColor;
        Color shadowColor;
        int const alpha  = getBaseColor().a;
        highlightColor   = faceColor + 0x303030;
        highlightColor.a = alpha;
        shadowColor      = faceColor - 0x303030;
        shadowColor.a    = alpha;

        if (isOpaque()) {
            // Fill the background around the content
            graphics->setColor(faceColor);
            graphics->fillRectangle(
                getBorderSize(),
                getBorderSize(),
                getWidth() - (2 * getBorderSize()),
                getHeight() - (2 * getBorderSize()));
        }
        if (mBackgroundWidget != nullptr) {
            Rectangle const rec(
                getBorderSize(),
                getBorderSize(),
                getWidth() - (2 * getBorderSize()),
                getHeight() - (2 * getBorderSize()));
            mBackgroundWidget->setDimension(rec);
            mBackgroundWidget->_draw(graphics);
        }
        if (getBorderSize() > 0) {
            drawBorder(graphics);
        }

        // draw inner/content border
        if (getInnerBorderSize() > 0) {
            drawInnerBorder(graphics);
        }

        // draw text
        int textX       = 0;
        int const textY = (static_cast<int>(getTitleBarHeight()) - getFont()->getHeight()) / 2;

        switch (getAlignment()) {
        case Graphics::Alignment::Left:
            textX = 0;
            break;
        case Graphics::Alignment::Center:
            textX = (getWidth() - 2 * getBorderSize() - getPaddingLeft() - getPaddingRight()) / 2;
            break;
        case Graphics::Alignment::Right:
            textX = getWidth() - getBorderSize() - getPaddingRight();
            break;
        default:
            throwException("Unknown alignment.");
        }
        // text clip area
        Rectangle const rec(
            getBorderSize() + getPaddingLeft(),
            getBorderSize() + getPaddingTop(),
            getWidth() - (2 * getBorderSize()) - getPaddingLeft() - getPaddingRight(),
            getTitleBarHeight() - 1);

        graphics->setColor(getForegroundColor());
        graphics->setFont(getFont());
        graphics->pushClipArea(rec);
        graphics->drawText(getCaption(), textX, textY, getAlignment());
        graphics->popClipArea();
    }

    void Window::mousePressed(MouseEvent& mouseEvent)
    {
        if (mouseEvent.getSource() != this) {
            return;
        }

        if (getParent() != nullptr) {
            getParent()->moveToTop(this);
        }

        mDragOffsetX = mouseEvent.getX();
        mDragOffsetY = mouseEvent.getY();

        int const height = getBorderSize() + getPaddingTop() + getTitleBarHeight();
        mMoved           = mouseEvent.getY() <= height;
    }

    void Window::mouseReleased(MouseEvent& /*mouseEvent*/)
    {
        mMoved = false;
    }

    void Window::mouseDragged(MouseEvent& mouseEvent)
    {
        if (mouseEvent.isConsumed() || mouseEvent.getSource() != this) {
            return;
        }

        if (isMovable() && mMoved) {
            setPosition(mouseEvent.getX() - mDragOffsetX + getX(), mouseEvent.getY() - mDragOffsetY + getY());
        }

        mouseEvent.consume();
    }

    void Window::adjustSize()
    {
        resizeToChildren();
        int const w = std::max(getFont()->getWidth(mCaption), getWidth()) + (2 * getBorderSize()) + getPaddingLeft() +
                      getPaddingRight() + (2 * getInnerBorderSize());
        int const h = getHeight() + (2 * getBorderSize()) + getPaddingTop() + getPaddingBottom() +
                      (2 * getInnerBorderSize()) + getTitleBarHeight();
        setSize(w, h);
    }

    Rectangle Window::getChildrenArea()
    {
        Rectangle rec;
        rec.x      = getBorderSize() + getPaddingLeft() + getInnerBorderSize();
        rec.y      = getBorderSize() + getPaddingTop() + getInnerBorderSize() + getTitleBarHeight();
        rec.width  = getWidth() - 2 * getBorderSize() - getPaddingLeft() - getPaddingRight() - 2 * getInnerBorderSize();
        rec.height = getHeight() - 2 * getBorderSize() - getPaddingTop() - getPaddingBottom() -
                     2 * getInnerBorderSize() - getTitleBarHeight();
        return rec;
    }

    void Window::setMovable(bool movable)
    {
        mMovable = movable;
    }

    bool Window::isMovable() const
    {
        return mMovable;
    }

    void Window::setOpaque(bool opaque)
    {
        mOpaque = opaque;
    }

    bool Window::isOpaque() const
    {
        return mOpaque;
    }
} // namespace fcn
