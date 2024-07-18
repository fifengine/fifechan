/***************************************************************************
 *   Copyright (c) 2017-2019 by the fifechan team                               *
 *   https://github.com/fifengine/fifechan                                 *
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

#include <algorithm>

#include "fifechan/widgets/imagebutton.hpp"

#include "fifechan/exception.hpp"
#include "fifechan/font.hpp"
#include "fifechan/graphics.hpp"
#include "fifechan/image.hpp"

namespace fcn
{
    ImageButton::ImageButton()
    {
        mImages         = std::vector<Image const *>(6, static_cast<Image const *>(0));
        mInternalImages = std::vector<bool>(6, false);
        adjustSize();
    }

    ImageButton::ImageButton(std::string const & filename)
    {
        mImages         = std::vector<Image const *>(6, static_cast<Image const *>(0));
        mInternalImages = std::vector<bool>(6, false);
        setUpImage(filename);
    }

    ImageButton::ImageButton(Image const * image)
    {
        mImages         = std::vector<Image const *>(6, static_cast<Image const *>(0));
        mInternalImages = std::vector<bool>(6, false);
        setUpImage(image);
    }

    ImageButton::~ImageButton()
    {
        for (unsigned int i = 0; i < 6; ++i) {
            if (mInternalImages[i]) {
                delete mImages[i];
            }
        }
    }

    void ImageButton::setImage(std::string const & filename, ImageType type)
    {
        if (mInternalImages[type]) {
            delete mImages[type];
        }
        Image const * image = Image::load(filename);
        if (image) {
            mImages[type]         = image;
            mInternalImages[type] = true;
        } else {
            mImages[type]         = 0;
            mInternalImages[type] = false;
        }
        adjustSize();
    }

    void ImageButton::setImage(Image const * image, ImageType type)
    {
        if (mInternalImages[type]) {
            delete mImages[type];
        }
        mImages[type]         = image;
        mInternalImages[type] = false;
        adjustSize();
    }

    void ImageButton::setUpImage(std::string const & filename)
    {
        setImage(filename, Image_Up);
    }

    void ImageButton::setUpImage(Image const * image)
    {
        setImage(image, Image_Up);
    }

    Image const * ImageButton::getUpImage() const
    {
        return mImages[Image_Up];
    }

    void ImageButton::setDownImage(std::string const & filename)
    {
        setImage(filename, Image_Down);
    }

    void ImageButton::setDownImage(Image const * image)
    {
        setImage(image, Image_Down);
    }

    Image const * ImageButton::getDownImage() const
    {
        return mImages[Image_Down];
    }

    void ImageButton::setHoverImage(std::string const & filename)
    {
        setImage(filename, Image_Hover);
    }

    void ImageButton::setHoverImage(Image const * image)
    {
        setImage(image, Image_Hover);
    }

    Image const * ImageButton::getHoverImage() const
    {
        return mImages[Image_Hover];
    }

    void ImageButton::setInactiveUpImage(std::string const & filename)
    {
        setImage(filename, Image_Up_De);
    }

    void ImageButton::setInactiveUpImage(Image const * image)
    {
        setImage(image, Image_Up_De);
    }

    Image const * ImageButton::getInactiveUpImage() const
    {
        return mImages[Image_Up_De];
    }

    void ImageButton::setInactiveDownImage(std::string const & filename)
    {
        setImage(filename, Image_Down_De);
    }

    void ImageButton::setInactiveDownImage(Image const * image)
    {
        setImage(image, Image_Down_De);
    }

    Image const * ImageButton::getInactiveDownImage() const
    {
        return mImages[Image_Down_De];
    }

    void ImageButton::setInactiveHoverImage(std::string const & filename)
    {
        setImage(filename, Image_Hover_De);
    }

    void ImageButton::setInactiveHoverImage(Image const * image)
    {
        setImage(image, Image_Hover_De);
    }

    Image const * ImageButton::getInactiveHoverImage() const
    {
        return mImages[Image_Hover_De];
    }

    void ImageButton::resizeToContent(bool recursiv)
    {
        adjustSize();
    }

    void ImageButton::adjustSize()
    {
        int w = 0;
        int h = 0;
        for (unsigned int i = 0; i < 6; ++i) {
            Image const * img = mImages[i];
            if (img) {
                w = std::max(w, img->getWidth());
                h = std::max(h, img->getHeight());
            }
        }
        if (!getCaption().empty()) {
            w = std::max(getFont()->getWidth(getCaption()), w);
            h = std::max(getFont()->getHeight(), h);
        }
        w += 2 * getBorderSize() + getPaddingLeft() + getPaddingRight();
        h += 2 * getBorderSize() + getPaddingTop() + getPaddingBottom();
        setSize(w, h);
    }

    void ImageButton::draw(Graphics* graphics)
    {
        // draw border or frame
        if (getBorderSize() > 0) {
            if (isFocused() && (getSelectionMode() & Widget::Selection_Border) == Widget::Selection_Border) {
                drawSelectionFrame(graphics);
            } else {
                drawBorder(graphics);
            }
        }

        Rectangle offsetRec(getBorderSize(), getBorderSize(), 2 * getBorderSize(), 2 * getBorderSize());
        // fetch the image, down, hover or up
        Image const * img = !isActive() && getInactiveUpImage() ? getInactiveUpImage() : getUpImage();
        if (isPressed()) {
            offsetRec.x += getDownXOffset();
            offsetRec.y += getDownYOffset();
            if (!isActive()) {
                if (getInactiveDownImage()) {
                    img = getInactiveDownImage();
                }
            } else {
                img = getDownImage() ? getDownImage() : getUpImage();
            }
        } else if (mHasMouse) {
            if (!isActive()) {
                if (getInactiveHoverImage()) {
                    img = getInactiveHoverImage();
                }
            } else {
                img = getHoverImage() ? getHoverImage() : getUpImage();
            }
        }
        // render foreground image or color rectangle
        if (img) {
            graphics->drawImage(
                img, 0, 0, offsetRec.x, offsetRec.y, getWidth() - offsetRec.width, getHeight() - offsetRec.height);
        } else {
            Color faceColor = getBaseColor();
            if (isFocused() && ((getSelectionMode() & Widget::Selection_Background) == Widget::Selection_Background)) {
                faceColor = getSelectionColor();
            }
            int alpha = faceColor.a;

            if (isPressed()) {
                faceColor   = faceColor - 0x303030;
                faceColor.a = alpha;
            }
            if (!isActive()) {
                int color   = static_cast<int>(faceColor.r * 0.3 + faceColor.g * 0.59 + faceColor.b * 0.11);
                faceColor.r = color;
                faceColor.g = color;
                faceColor.b = color;
            }

            graphics->setColor(faceColor);
            graphics->fillRectangle(
                offsetRec.x, offsetRec.y, getWidth() - offsetRec.width, getHeight() - offsetRec.height);
        }

        // render caption if it exits
        if (!getCaption().empty()) {
            int textX;
            int textY =
                offsetRec.y + getPaddingTop() +
                (getHeight() - offsetRec.height - getPaddingTop() - getPaddingBottom() - getFont()->getHeight()) / 2;
            switch (getAlignment()) {
            case Graphics::Left:
                textX = offsetRec.x + getPaddingLeft();
                break;
            case Graphics::Center:
                textX = offsetRec.x + getPaddingLeft() +
                        (getWidth() - offsetRec.width - getPaddingLeft() - getPaddingRight()) / 2;
                break;
            case Graphics::Right:
                textX = getWidth() - offsetRec.x - getPaddingRight();
                break;
            default:
                fcn::throwException("Unknown alignment.", static_cast<const char*>(__FUNCTION__), __FILE__, __LINE__);
            }
            // set font and color
            graphics->setFont(getFont());
            graphics->setColor(getForegroundColor());
            graphics->drawText(getCaption(), textX, textY, getAlignment());
        }
    }
} // namespace fcn
