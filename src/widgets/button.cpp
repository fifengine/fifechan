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

#include "fifechan/widgets/button.hpp"

#include "fifechan/exception.hpp"
#include "fifechan/font.hpp"
#include "fifechan/graphics.hpp"
#include "fifechan/key.hpp"
#include "fifechan/mouseevent.hpp"
#include "fifechan/mouseinput.hpp"

namespace fcn
{
    Button::Button()
        : mHasMouse(false),
          mKeyPressed(false),
          mMousePressed(false),
          mState(true),
          mAlignment(Graphics::Center),
          mXOffset(1),
          mYOffset(1) {
        setFocusable(true);
        adjustSize();

        addMouseListener(this);
        addKeyListener(this);
        addFocusListener(this);
        addWidgetListener(this);
    }

    Button::Button(const std::string& caption)
            : mCaption(caption),
              mHasMouse(false),
              mKeyPressed(false),
              mMousePressed(false),
              mState(true),
              mAlignment(Graphics::Center),
              mXOffset(1),
              mYOffset(1) {
        setFocusable(true);
        adjustSize();

        addMouseListener(this);
        addKeyListener(this);
        addFocusListener(this);
        addWidgetListener(this);
    }

    Button::~Button() {

    }

    void Button::setCaption(const std::string& caption) {
        mCaption = caption;
        adjustSize();
    }

    const std::string& Button::getCaption() const {
        return mCaption;
    }

    void Button::setActive(bool state) {
        mState = state;
    }

    bool Button::isActive() const {
        return mState;
    }

    void Button::setAlignment(Graphics::Alignment alignment) {
        mAlignment = alignment;
    }

    Graphics::Alignment Button::getAlignment() const {
        return mAlignment;
    }

    void Button::setDownXOffset(int offset) {
        mXOffset = offset;
    }

    int Button::getDownXOffset() const {
        return mXOffset;
    }

    void Button::setDownYOffset(int offset) {
        mYOffset = offset;
    }

    int Button::getDownYOffset() const {
        return mYOffset;
    }

    void Button::setDownOffset(int x, int y) {
        mXOffset = x;
        mYOffset = y;
    }

    void Button::fontChanged() {
        adjustSize();
    }

    void Button::draw(Graphics* graphics) {
        bool active = isFocused();
        Color faceColor = getBaseColor();
        if (active && ((getSelectionMode() & Widget::Selection_Background) == Widget::Selection_Background)) {
            faceColor = getSelectionColor();
        }
        int alpha = faceColor.a;

        if (isPressed()) {
            faceColor = faceColor - 0x303030;
            faceColor.a = alpha;
        }
        if (!isActive()) {
            int color = static_cast<int>(faceColor.r * 0.3 + faceColor.g * 0.59 + faceColor.b * 0.11);
            faceColor.r = color;
            faceColor.g = color;
            faceColor.b = color;
        }

        graphics->setColor(faceColor);
        Rectangle offsetRec(getBorderSize(), getBorderSize(), 2 * getBorderSize(), 2 * getBorderSize());
        graphics->fillRectangle(offsetRec.x, offsetRec.y, getWidth() - offsetRec.width, getHeight() - offsetRec.height);

        if (getBorderSize() > 0) {
            if (active && (getSelectionMode() & Widget::Selection_Border) == Widget::Selection_Border) {
                drawSelectionFrame(graphics);
            } else {
                drawBorder(graphics);
            }
        }

        int textX;
        int textY = offsetRec.y + getPaddingTop() + (getHeight() - offsetRec.height - getPaddingTop() - getPaddingBottom() - getFont()->getHeight()) / 2;
        switch (getAlignment()) {
          case Graphics::Left:
              textX = offsetRec.x + getPaddingLeft();
              break;
          case Graphics::Center:
              textX = offsetRec.x + getPaddingLeft() + (getWidth() - offsetRec.width - getPaddingLeft() - getPaddingRight()) / 2;
              break;
          case Graphics::Right:
              textX = getWidth() - offsetRec.x - getPaddingRight();
              break;
          default:
              throw FCN_EXCEPTION("Unknown alignment.");
        }

        // set font and color
        graphics->setFont(getFont());
        graphics->setColor(getForegroundColor());
        if (isPressed()) {
            graphics->drawText(getCaption(), textX + getDownXOffset(), textY + getDownYOffset(), getAlignment());
        } else {
            graphics->drawText(getCaption(), textX, textY, getAlignment());
        }
    }

    void Button::resizeToContent(bool recursiv) {
        adjustSize();
    }

    void Button::adjustSize() {
        int w = getFont()->getWidth(mCaption) + 2 * getBorderSize() + getPaddingLeft() + getPaddingRight();
        int h = getFont()->getHeight() + 2 * getBorderSize() + getPaddingTop() + getPaddingBottom();
        setSize(w, h);
    }

    bool Button::isPressed() const {
        if (mMousePressed) {
            return mHasMouse;
        }
        return mKeyPressed;
    }

    void Button::mousePressed(MouseEvent& mouseEvent) {
        if (mouseEvent.getButton() == MouseEvent::Left) {
            mMousePressed = true;
            mouseEvent.consume();
        }
    }

    void Button::mouseExited(MouseEvent& mouseEvent) {
        mHasMouse = false;
    }

    void Button::mouseEntered(MouseEvent& mouseEvent) {
        mHasMouse = true;
    }

    void Button::mouseReleased(MouseEvent& mouseEvent) {
        if (mouseEvent.getButton() == MouseEvent::Left && mMousePressed && mHasMouse) {
            mMousePressed = false;
            distributeActionEvent();
            mouseEvent.consume();
        } else if (mouseEvent.getButton() == MouseEvent::Left) {
            mMousePressed = false;
            mouseEvent.consume();
        }
    }

    void Button::mouseDragged(MouseEvent& mouseEvent) {
        mouseEvent.consume();
    }

    void Button::keyPressed(KeyEvent& keyEvent) {
        Key key = keyEvent.getKey();

        if (key.getValue() == Key::Enter || key.getValue() == Key::Space) {
            mKeyPressed = true;
            keyEvent.consume();
        }
    }

    void Button::keyReleased(KeyEvent& keyEvent) {
        Key key = keyEvent.getKey();

        if ((key.getValue() == Key::Enter || key.getValue() == Key::Space) && mKeyPressed) {
            mKeyPressed = false;
            distributeActionEvent();
            keyEvent.consume();
        }
    }

    void Button::focusLost(const Event& event) {
        mMousePressed = false;
        mKeyPressed = false;
        mHasMouse = false;
    }

    void Button::ancestorHidden(const Event& e) {
        mMousePressed = false;
        mKeyPressed = false;
        mHasMouse = false;
    }
}
