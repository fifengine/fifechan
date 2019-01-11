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

#include <fifechan/image.hpp>
#include <fifechan/exception.hpp>
#include <fifechan/graphics.hpp>
#include <fifechan/widgets/iconprogressbar.hpp>

namespace fcn
{
    IconProgressBar::IconProgressBar()
    :
    mImage(NULL),
    mInternalImage(false),
    mMaxIcons(0),
    mIconCounter(0),
    mOrientation(HORIZONTAL),
    mOpaque(true) {
    }
    
    IconProgressBar::IconProgressBar(Image* image, int maxIcons)
    :
    mImage(image),
    mInternalImage(false),
    mMaxIcons(maxIcons),
    mIconCounter(0),
    mOrientation(HORIZONTAL),
    mOpaque(true) {
        adjustSize();
    }
    
    IconProgressBar::IconProgressBar(const std::string& filename, int maxIcons)
    :
    mImage(NULL),
    mInternalImage(true),
    mMaxIcons(maxIcons),
    mIconCounter(0),
    mOrientation(HORIZONTAL),
    mOpaque(true) {
        mImage = Image::load(filename);
        adjustSize();
    }
    
    IconProgressBar::~IconProgressBar()
    {        
        if(mInternalImage)
        {
            delete mImage;
        }
    }
    
    void IconProgressBar::draw(Graphics* graphics)
    {
        bool active = isFocused();

        if (isOpaque()) {
            // Fill the background around the content
            if (active && ((getSelectionMode() & Widget::Selection_Background) == Widget::Selection_Background)) {
                graphics->setColor(getSelectionColor());
            } else {
                graphics->setColor(getBackgroundColor());
            }
            graphics->fillRectangle(getBorderSize(), getBorderSize(),
                getWidth() - 2 * getBorderSize(), getHeight() - 2 * getBorderSize());
        }
        // draw border or frame
        if (getBorderSize() > 0) {
            if (active && (getSelectionMode() & Widget::Selection_Border) == Widget::Selection_Border) {
                drawSelectionFrame(graphics);
            } else {
                drawBorder(graphics);
            }
        }
        if (!mImage) {
            return;
        }
        // draw "icons"
        int x = getBorderSize() + getPaddingLeft();
        int y = getBorderSize() + getPaddingTop();
        if (mOrientation == HORIZONTAL) {
            for (int i = 0; i < mIconCounter; i++) {
                graphics->drawImage(mImage, x, y);
                x += mImage->getWidth();
            }
        } else {
            for (int i = 0; i < mIconCounter; i++) {
                graphics->drawImage(mImage, x, y);
                y += mImage->getHeight();
            }
        }
    }
        
    void IconProgressBar::setOpaque(bool opaque)
    {
        mOpaque = opaque;
    }
    
    bool IconProgressBar::isOpaque() const
    {
        return mOpaque;
    }
    
    void IconProgressBar::setImage(Image* image)
    {
        if(mInternalImage)
        {
            delete mImage;
        }
        
        mInternalImage = false;
        mImage = image;
        
        adjustSize();
    }
    
    const Image* IconProgressBar::getImage() const
    {
        return mImage;
    }
    
    void IconProgressBar::setMaxIcons(int maxIcons)
    {
        mMaxIcons = maxIcons;
        adjustSize();
    }
    
    int IconProgressBar::getMaxIcons() const
    {
        return mMaxIcons;
    }
    
    void IconProgressBar::setOrientation(Orientation orientation)
    {
        if (mOrientation != orientation) {
            if (orientation != HORIZONTAL && orientation != VERTICAL) {
                throw FCN_EXCEPTION("Unknown orientation type in IconProgressBar object");
                return;
            }
            mOrientation = orientation;
            adjustSize();
        }
    }
    
    IconProgressBar::Orientation IconProgressBar::getOrientation() const
    {
        return mOrientation;
    }
    
    void IconProgressBar::advance()
    {
        mIconCounter = (mIconCounter + 1) % (mMaxIcons+1);
    }
    
    void IconProgressBar::reset()
    {
        mIconCounter = 0;
    }
    
    void IconProgressBar::setIconCount(int icons) {
        mIconCounter = icons % (mMaxIcons+1);
    }

    int IconProgressBar::getIconCount() const {
        return mIconCounter;
    }

    void IconProgressBar::resizeToContent(bool recursiv) {
        adjustSize();
    }

    void IconProgressBar::adjustSize() {
        int w = 0;
        int h = 0;
        if (mImage) {
            w = mImage->getWidth();
            h = mImage->getHeight();
            if (mOrientation == HORIZONTAL) {
                w *= mMaxIcons;
            } else {
                h *= mMaxIcons;
            }
        }
        w += 2 * getBorderSize() + getPaddingLeft() + getPaddingRight();
        h += 2 * getBorderSize() + getPaddingTop() + getPaddingBottom();
        setSize(w, h);
    }
};