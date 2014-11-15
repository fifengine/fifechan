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

#include <fifechan/image.hpp>
#include <fifechan/exception.hpp>
#include <fifechan/graphics.hpp>
#include <fifechan/rectangle.hpp>
#include <fifechan/widgets/imageprogressbar.hpp>

namespace fcn
{
    ImageProgressBar::ImageProgressBar()
    :
    mBarImage(NULL),
    mForegroundImage(NULL),
    mInternalImage(false),
    mMaxValue(100),
    mValue(0),
    mOrientation(HORIZONTAL),
    mOpaque(true) {
    }
    
    ImageProgressBar::ImageProgressBar(Image* image, int maxValue)
    :
    mBarImage(image),
    mForegroundImage(NULL),
    mInternalImage(false),
    mMaxValue(maxValue),
    mValue(0),
    mOrientation(HORIZONTAL),
    mOpaque(true) {
        adjustSize();
    }
    
    ImageProgressBar::ImageProgressBar(const std::string& filename, int maxValue)
    :
    mBarImage(NULL),
    mForegroundImage(NULL),
    mInternalImage(true),
    mMaxValue(maxValue),
    mValue(0),
    mOrientation(HORIZONTAL),
    mOpaque(true) {
        mBarImage = Image::load(filename);
        adjustSize();
    }
    
    ImageProgressBar::~ImageProgressBar() {        
        if(mInternalImage) {
            delete mBarImage;
        }
    }
    
    void ImageProgressBar::draw(Graphics* graphics) {
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
        if (mBarImage) {
            if (getOrientation() == HORIZONTAL) {
                Rectangle rec = Rectangle(getBorderSize() + getPaddingLeft(), getBorderSize() + getPaddingTop(),
                    mBarImage->getWidth() * mValue/mMaxValue, mBarImage->getHeight());
                graphics->pushClipArea(rec);
                graphics->drawImage(mBarImage, 0, 0);
                graphics->popClipArea();
            } else {
                Rectangle rec = Rectangle(getBorderSize() + getPaddingLeft(),
                    getBorderSize() + getPaddingTop() + (mBarImage->getHeight() - mBarImage->getHeight() * mValue/mMaxValue),
                    mBarImage->getWidth(), mBarImage->getHeight() * mValue/mMaxValue);
                graphics->pushClipArea(rec);
                graphics->drawImage(mBarImage, 0, 0);
                graphics->popClipArea();
            }
        }

        if (mForegroundImage) {
            int x = getBorderSize() + getPaddingLeft();
            int y = getBorderSize() + getPaddingTop();
            graphics->drawImage(mForegroundImage, x, y);
        }
    }
        
    void ImageProgressBar::setOpaque(bool opaque) {
        mOpaque = opaque;
    }
    
    bool ImageProgressBar::isOpaque() const {
        return mOpaque;
    }
    
    void ImageProgressBar::setBarImage(Image* image) {
        if(mInternalImage) {
            delete mBarImage;
        }
        
        mInternalImage = false;
        mBarImage = image;
        
        adjustSize();
    }
    
    const Image* ImageProgressBar::getBarImage() const {
        return mBarImage;
    }

    void ImageProgressBar::setForegroundImage(Image* image) {
        mForegroundImage = image;
        adjustSize();
    }
    
    const Image* ImageProgressBar::getForegroundImage() const {
        return mForegroundImage;
    }
    
    void ImageProgressBar::setOrientation(Orientation orientation) {
        if (mOrientation != orientation) {
            if (orientation != HORIZONTAL && orientation != VERTICAL) {
                throw FCN_EXCEPTION("Unknown orientation type in ImageProgressBar object");
                return;
            }
            mOrientation = orientation;
        }
    }
    
    ImageProgressBar::Orientation ImageProgressBar::getOrientation() const {
        return mOrientation;
    }

    void ImageProgressBar::setMaxValue(int32_t value) {
        mMaxValue = value;
    }
    
    int ImageProgressBar::getMaxValue() const {
        return mMaxValue;
    }

    void ImageProgressBar::setValue(int32_t value) {
        if (value > mMaxValue) {
            mValue = mMaxValue;
        } else if (value < 0) {
            mValue = 0;
        } else {
            mValue = value;
        }
    }
    
    int ImageProgressBar::getValue() const {
        return mValue;
    }

    void ImageProgressBar::resizeToContent(bool recursiv) {
        adjustSize();
    }

    void ImageProgressBar::adjustSize() {
        int w = 0;
        int h = 0;
        if (mBarImage) {
            w = mBarImage->getWidth();
            h = mBarImage->getHeight();
        }
        w += 2 * getBorderSize() + getPaddingLeft() + getPaddingRight();
        h += 2 * getBorderSize() + getPaddingTop() + getPaddingBottom();
        setSize(w, h);
    }
};