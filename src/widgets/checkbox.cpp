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

#include "fifechan/widgets/checkbox.hpp"

#include "fifechan/exception.hpp"
#include "fifechan/font.hpp"
#include "fifechan/graphics.hpp"
#include "fifechan/key.hpp"
#include "fifechan/mouseinput.hpp"

namespace fcn
{

    CheckBox::CheckBox()
    {
        setSelected(false);
        setMarkerMode(Checkmark);

        setFocusable(true);
        addMouseListener(this);
        addKeyListener(this);
    }

    CheckBox::CheckBox(const std::string &caption, bool selected)
    {
        setCaption(caption);
        setSelected(selected);
        setMarkerMode(Checkmark);

        setFocusable(true);
        addMouseListener(this);
        addKeyListener(this);

        adjustSize();
    }

    void CheckBox::draw(Graphics* graphics)
    {
        drawBox(graphics);

        graphics->setFont(getFont());
        graphics->setColor(getForegroundColor());

        int h = getHeight() - 2 * getBorderSize() - getPaddingTop() - getPaddingBottom();
        int textX = getBorderSize() + getPaddingLeft() + h;
        int textY = getBorderSize() + getPaddingTop() + (h - getFont()->getHeight()) / 2;
        graphics->drawText(getCaption(), textX, textY);
    }

    void CheckBox::drawBox(Graphics *graphics)
    {
        bool active = isFocused();
        int h = getHeight() - 2 * getBorderSize() - getPaddingTop() - getPaddingBottom();
        // rec for inner box
        Rectangle rec(getBorderSize() + getPaddingLeft(), getBorderSize() + getPaddingTop(), h, h);
        // draw background
        Color faceColor = getBaseColor();
        if (active && ((getSelectionMode() & Widget::Selection_Background) == Widget::Selection_Background)) {
            faceColor = getSelectionColor();
        }
        graphics->setColor(faceColor);
        graphics->fillRectangle(getBorderSize(), getBorderSize(), getWidth() - 2 * getBorderSize(), getHeight() - 2 * getBorderSize());
        // draw inner box background
        graphics->setColor(getBackgroundColor());
        graphics->fillRectangle(rec.x, rec.y, h, h);
        // draw border
        if (getBorderSize() > 0) {
            if (active && (getSelectionMode() & Widget::Selection_Border) == Widget::Selection_Border) {
                drawSelectionFrame(graphics);
            } else {
                drawBorder(graphics);
            }
        }
        // border around the inner box background
        int alpha = getBaseColor().a;
        faceColor.a = alpha;
        Color highlightColor = faceColor + 0x303030;
        highlightColor.a = alpha;
        Color shadowColor = faceColor - 0x303030;
        shadowColor.a = alpha;

        graphics->setColor(shadowColor);
        graphics->drawLine(rec.x, rec.y, h-1, rec.y);
        graphics->drawLine(rec.x, rec.y, rec.x, h-1);

        graphics->setColor(highlightColor);
        graphics->drawLine(h-1, rec.x, h-1, h-1);
        graphics->drawLine(rec.y, h-1, h-1, h-1);

        // draws marker
        if (mSelected) {
            graphics->setColor(getForegroundColor());
            switch(mMode) {
                case Checkmark:
                    drawCheckmark(graphics, rec);
                    break;
                case Cross:
                    drawCross(graphics, rec);
                    break;
                case Dot:
                    drawDot(graphics, rec);
                    break;
                default:
                    throw FCN_EXCEPTION("Unknown marker.");
            }
        }
    }

    void CheckBox::drawCheckmark(Graphics* graphics, const Rectangle& rec) {
        graphics->drawLine(rec.x+3, rec.y+3, rec.x+3, rec.height-3);
        graphics->drawLine(rec.x+4, rec.y+4, rec.x+4, rec.height-2);
        graphics->drawLine(rec.x+5, rec.height-3, rec.width-2, rec.y+4);
        graphics->drawLine(rec.x+5, rec.height-4, rec.width-4, rec.y+5);
    }

    void CheckBox::drawCross(Graphics* graphics, const Rectangle& rec) {
        graphics->drawLine(rec.x+2, rec.y+2, rec.width-3, rec.height-3);
        graphics->drawLine(rec.x+2, rec.y+3, rec.width-4, rec.height-3);
        graphics->drawLine(rec.x+2, rec.height-3, rec.width-3, rec.y+2);
        graphics->drawLine(rec.x+3, rec.height-3, rec.width-3, rec.y+3);
    }

    void CheckBox::drawDot(Graphics* graphics, const Rectangle& rec) {
        Point p(rec.x+rec.width/2, rec.y+rec.height/2);
        graphics->drawFillCircle(p, (rec.width-3)/2);
    }

    bool CheckBox::isSelected() const
    {
        return mSelected;
    }

    void CheckBox::setSelected(bool selected)
    {
        mSelected = selected;
    }

    const std::string &CheckBox::getCaption() const
    {
        return mCaption;
    }

    void CheckBox::setCaption(const std::string& caption)
    {
        mCaption = caption;
    }

    CheckBox::MarkerMode CheckBox::getMarkerMode() const {
        return mMode;
    }

    void CheckBox::setMarkerMode(CheckBox::MarkerMode mode) {
        mMode = mode;
    }

    void CheckBox::keyPressed(KeyEvent& keyEvent)
    {
        Key key = keyEvent.getKey();

        if (key.getValue() == Key::Enter ||
            key.getValue() == Key::Space)
        {
            toggleSelected();
            keyEvent.consume();
        }
    }

    void CheckBox::mouseClicked(MouseEvent& mouseEvent)
    {
        if (mouseEvent.getButton() == MouseEvent::Left)
        {
            toggleSelected();
        }
    }

    void CheckBox::mouseDragged(MouseEvent& mouseEvent)
    {
        mouseEvent.consume();
    }

    void CheckBox::resizeToContent(bool recursiv) {
        adjustSize();
    }

    void CheckBox::adjustSize()
    {
        // w + getFont()->getHeight() ???
        int w = getFont()->getWidth(mCaption) + 2 * getBorderSize() + getPaddingLeft() + getPaddingRight();
        int h = getFont()->getHeight() + 2 * getBorderSize() + getPaddingTop() + getPaddingBottom();
        setSize(w, h);
    }

    void CheckBox::toggleSelected()
    {
        mSelected = !mSelected;
        distributeActionEvent();
    }
}

