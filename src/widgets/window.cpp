/***************************************************************************
 *   Copyright (C) 2012 by the fifechan team                               *
 *   http://fifechan.github.com/fifechan                                   *
 *   This file is part of fifechan.                                        *
 *                                                                         *
 *   fifechan is free software; you can redistribute it and/or             *
 *   modify it under the terms of the GNU Lesser General Public            *
 *   License as published by the Free Software Foundation; either          *
 *   version 2.1 of the License, or (at your option) any later version.    *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *   Lesser General Public License for more details.                       *
 *                                                                         *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this library; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA          *
 ***************************************************************************/

/*      _______   __   __   __   ______   __   __   _______   __   __
 *     / _____/\ / /\ / /\ / /\ / ____/\ / /\ / /\ / ___  /\ /  |\/ /\
 *    / /\____\// / // / // / // /\___\// /_// / // /\_/ / // , |/ / /
 *   / / /__   / / // / // / // / /    / ___  / // ___  / // /| ' / /
 *  / /_// /\ / /_// / // / // /_/_   / / // / // /\_/ / // / |  / /
 * /______/ //______/ //_/ //_____/\ /_/ //_/ //_/ //_/ //_/ /|_/ /
 * \______\/ \______\/ \_\/ \_____\/ \_\/ \_\/ \_\/ \_\/ \_\/ \_\/
 *
 * Copyright (c) 2004 - 2008 Olof Naessén and Per Larsson
 *
 *
 * Per Larsson a.k.a finalman
 * Olof Naessén a.k.a jansem/yakslem
 *
 * Visit: http://guichan.sourceforge.net
 *
 * License: (BSD)
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name of Guichan nor the names of its contributors may
 *    be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * For comments regarding functions please see the header file.
 */

#include "fifechan/widgets/window.hpp"

#include "fifechan/exception.hpp"
#include "fifechan/font.hpp"
#include "fifechan/graphics.hpp"
#include "fifechan/mouseinput.hpp"

namespace fcn
{
    Window::Window()
            :mMoved(false)
    {
        setBorderSize(1);
        setInnerBorderSize(1);
        setPadding(2);
        setTitleBarHeight(16);
        setAlignment(Graphics::Center);
        addMouseListener(this);
        setMovable(true);
        setOpaque(true);
    }

    Window::Window(const std::string& caption)
            :mMoved(false)
    {
        setCaption(caption);
        setBorderSize(1);
        setInnerBorderSize(1);
        setPadding(2);
        setTitleBarHeight(16);
        setAlignment(Graphics::Center);
        addMouseListener(this);
        setMovable(true);
        setOpaque(true);
    }

    Window::~Window()
    {
    }

    void Window::setTitleBarHeight(unsigned int height)
    {
        mTitleBarHeight = height;
    }

    unsigned int Window::getTitleBarHeight() const
    {
        return mTitleBarHeight;
    }

    void Window::setInnerBorderSize(unsigned int border) {
        mInnerBorder = border;
    }

    unsigned int Window::getInnerBorderSize() const {
        return mInnerBorder;
    }

    void Window::setCaption(const std::string& caption)
    {
        mCaption = caption;
    }

    const std::string& Window::getCaption() const
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

    void Window::drawInnerBorder(Graphics* graphics) {
        const Color &faceColor = getBaseColor();
        Color highlightColor, shadowColor;
        const int alpha = getBaseColor().a;
        highlightColor = faceColor + 0x303030;
        highlightColor.a = alpha;
        shadowColor = faceColor - 0x303030;
        shadowColor.a = alpha;

        int x = getXOffset() + getPaddingLeft();
        int y = getYOffset() + getPaddingTop() + getTitleBarHeight();
        int width = getWidth() - getMarginRight() - getBorderSize() - getPaddingRight() - 1;
        int height = getHeight() - getMarginBottom() - getBorderSize() - getPaddingBottom() - 1;

        unsigned int i;
        for (i = 0; i < getInnerBorderSize(); ++i) {
            graphics->setColor(shadowColor);
            graphics->drawLine(x+i, y+i, width-i, y+i);
            graphics->drawLine(x+i, y+i+1, x+i, height-i-1);
            graphics->setColor(highlightColor);
            graphics->drawLine(width-i, y+i+1, width-i, height-i);
            graphics->drawLine(x+i, height-i, width-i-1, height-i);
        }
    }

    void Window::draw(Graphics* graphics) {
        const Color &faceColor = getBaseColor();
        Color highlightColor, shadowColor;
        const int alpha = getBaseColor().a;
        highlightColor = faceColor + 0x303030;
        highlightColor.a = alpha;
        shadowColor = faceColor - 0x303030;
        shadowColor.a = alpha;

        if (isOpaque()) {
            // Fill the background around the content
            graphics->setColor(faceColor);
            graphics->fillRectangle(getXOffset(), getYOffset(), getWidth() + getWOffset(), getHeight() + getHOffset());
        }
        if (mBackgroundWidget) {
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
        int textX;
        int textY = ((int)getTitleBarHeight() - getFont()->getHeight()) / 2;

        switch (getAlignment())
        {
          case Graphics::Left:
              textX = getPaddingLeft();
              break;
          case Graphics::Center:
              textX = (getWidth() + getWOffset()) / 2;
              break;
          case Graphics::Right:
              textX = (getWidth() + getWOffset()) - getPaddingRight();
              break;
          default:
              throw FCN_EXCEPTION("Unknown alignment.");
        }
        // text clip area
        Rectangle rec(getXOffset(), getYOffset(), getWidth() + getWOffset(), getTitleBarHeight() - 1);

        graphics->setColor(getForegroundColor());
        graphics->setFont(getFont());
        graphics->pushClipArea(rec);
        graphics->drawText(getCaption(), textX, textY, getAlignment());
        graphics->popClipArea();
    }

    void Window::mousePressed(MouseEvent& mouseEvent)
    {
        if (mouseEvent.getSource() != this)
        {
            return;
        }

        if (getParent() != NULL)
        {
            getParent()->moveToTop(this);
        }

        mDragOffsetX = mouseEvent.getX();
        mDragOffsetY = mouseEvent.getY();

        int height = getMarginTop() + getBorderSize() + getPaddingTop() + getTitleBarHeight();
        mMoved = mouseEvent.getY() <= height;
    }

    void Window::mouseReleased(MouseEvent& mouseEvent)
    {
        mMoved = false;
    }

    void Window::mouseDragged(MouseEvent& mouseEvent)
    {
        if (mouseEvent.isConsumed() || mouseEvent.getSource() != this)
        {
            return;
        }

        if (isMovable() && mMoved)
        {
            setPosition(mouseEvent.getX() - mDragOffsetX + getX(),
                        mouseEvent.getY() - mDragOffsetY + getY());
        }

        mouseEvent.consume();
    }

    void Window::adjustSize() {
        resizeToChildren();
        if (mBackgroundWidget) {
            Rectangle rec(getXOffset(), getYOffset(), getWidth() + getWOffset(), getHeight() + getHOffset());
            mBackgroundWidget->setDimension(rec);
        }
        int w = getWidth() + getWOffset() - getPaddingLeft() - getPaddingRight() - 2*getInnerBorderSize();
        int h = getHeight() + getHOffset() - getPaddingTop() - getPaddingBottom() - 2*getInnerBorderSize() - getTitleBarHeight();
        setSize(w, h);
    }

    Rectangle Window::getChildrenArea()
    {
        Rectangle rec;
        rec.x = getXOffset() + getPaddingLeft() + getInnerBorderSize();
        rec.y = getYOffset() + getPaddingTop() + getInnerBorderSize() + getTitleBarHeight();
        rec.width = getWidth() + getWOffset() - getPaddingLeft() - getPaddingRight() - 2*getInnerBorderSize();
        rec.height = getHeight() + getHOffset() - getPaddingTop() - getPaddingBottom() - 2*getInnerBorderSize() - getTitleBarHeight();
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
}
