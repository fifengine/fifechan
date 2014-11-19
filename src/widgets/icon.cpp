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

#include "fifechan/widgets/icon.hpp"

#include "fifechan/graphics.hpp"
#include "fifechan/image.hpp"
#include "fifechan/rectangle.hpp"

namespace fcn
{
    Icon::Icon() :
        mImage(0),
        mInternalImage(false),
        mScale(false),
        mTile(false),
        mOpaque(true) {

        adjustSize();
    }

    Icon::Icon(const std::string& filename) :
        mImage(0),
        mInternalImage(false),
        mScale(false),
        mTile(false),
        mOpaque(true) {

        mImage = Image::load(filename);
        mInternalImage = true;
        adjustSize();
    }

    Icon::Icon(const Image* image) :
        mImage(image),
        mInternalImage(false),
        mScale(false),
        mTile(false),
        mOpaque(true) {

        adjustSize();
    }

    Icon::~Icon() {
        if (mInternalImage) {
            delete mImage;
        }
    }

    void Icon::setImage(const std::string& filename) {
        if (mInternalImage) {
            delete mImage;
        }

        mImage = Image::load(filename);
        mInternalImage = true;
        adjustSize();
    }

    void Icon::setImage(const Image* image) {
        if (mInternalImage) {
            delete mImage;
        }

        mImage = image;
        mInternalImage = false;
        adjustSize();
    }

    const Image* Icon::getImage() const {
        return mImage;
    }

    bool Icon::isScaling() const {
        return mScale;
    }

    void Icon::setScaling(bool scale) {
        mScale = scale;
    }

    bool Icon::isTiling() const {
        return mTile;
    }

    void Icon::setTiling(bool tile) {
        mTile = tile;
    }

    void Icon::setOpaque(bool opaque) {
        mOpaque = opaque;
    }

    bool Icon::isOpaque() const {
        return mOpaque;
    }

    void Icon::resizeToContent(bool recursiv) {
        adjustSize();
    }

    void Icon::adjustSize() {
        int w = 2 * getBorderSize() + getPaddingLeft() + getPaddingRight();
        int h = 2 * getBorderSize() + getPaddingTop() + getPaddingBottom();
        if (mImage) {
            w += mImage->getWidth();
            h += mImage->getHeight();
        }
        setSize(w, h);
    }

    void Icon::draw(Graphics* graphics) {
        // draw icon background
        if (mOpaque) {
            Color color = getBackgroundColor();
            if (isFocused() && ((getSelectionMode() & Widget::Selection_Background) == Widget::Selection_Background)) {
                color = getSelectionColor();
            }
            graphics->setColor(color);
            graphics->fillRectangle(Rectangle(getBorderSize(), getBorderSize(), getWidth() - 2 * getBorderSize(), getHeight() - 2 * getBorderSize()));
        }
        // draw icon image
        if (mImage) {
            Rectangle contentRect(getBorderSize() + getPaddingLeft(), getBorderSize() + getPaddingTop(),
                getWidth() - 2 * getBorderSize() - getPaddingLeft() - getPaddingRight(),
                getHeight() - 2 * getBorderSize() - getPaddingTop() - getPaddingBottom());

            // draw with widget or image size
            int w = mScale ? contentRect.width : mImage->getWidth();
            int h = mScale ? contentRect.height : mImage->getHeight();

            if (mTile && !mScale) {
                Rectangle rect(contentRect.x, contentRect.y, w, h);
                int tmpW = getWidth() - getBorderSize() - getPaddingRight();
                int tmpH = getHeight() - getBorderSize() - getPaddingBottom();
                while (rect.x < tmpW) {
                    rect.y = contentRect.y;
                    while (rect.y < tmpH) {
                        graphics->drawImage(mImage, 0, 0, rect.x, rect.y, rect.width, rect.height);
                        rect.y += rect.height;
                    }
                    rect.x += rect.width;
                }
            } else {
                graphics->drawImage(mImage, 0, 0, contentRect.x, contentRect.y, w, h);
            }
        }
        // draw border or frame
        if (getBorderSize() > 0) {
            if (isFocused() && (getSelectionMode() & Widget::Selection_Border) == Widget::Selection_Border) {
                drawSelectionFrame(graphics);
            } else {
                drawBorder(graphics);
            }
        }
    }
}
