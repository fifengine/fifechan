// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#include <fifechan/widgets/imageprogressbar.hpp>
#include <string>

#include <fifechan/exception.hpp>
#include <fifechan/graphics.hpp>
#include <fifechan/image.hpp>
#include <fifechan/rectangle.hpp>

namespace fcn
{
    ImageProgressBar::ImageProgressBar() :
        mBarImage(nullptr),
        mForegroundImage(nullptr),
        mInternalImage(false),
        mMaxValue(100),
        mValue(0),
        mOrientation(Orientation::Horizontal),
        mOpaque(true)
    {
    }

    ImageProgressBar::ImageProgressBar(Image* image, int maxValue) :
        mBarImage(image),
        mForegroundImage(nullptr),
        mInternalImage(false),
        mMaxValue(maxValue),
        mValue(0),
        mOrientation(Orientation::Horizontal),
        mOpaque(true)
    {
        adjustSize();
    }

    ImageProgressBar::ImageProgressBar(std::string const & filename, int maxValue) :
        mBarImage(nullptr),
        mForegroundImage(nullptr),
        mInternalImage(true),
        mMaxValue(maxValue),
        mValue(0),
        mOrientation(Orientation::Horizontal),
        mOpaque(true)
    {
        mBarImage = Image::load(filename);
        adjustSize();
    }

    ImageProgressBar::~ImageProgressBar()
    {
        if (mInternalImage) {
            delete mBarImage;
        }
    }

    void ImageProgressBar::draw(Graphics* graphics)
    {
        bool const active = isFocused();

        if (isOpaque()) {
            // Fill the background around the content
            if (active &&
                ((getSelectionMode() & Widget::SelectionMode::Background) == Widget::SelectionMode::Background)) {
                graphics->setColor(getSelectionColor());
            } else {
                graphics->setColor(getBackgroundColor());
            }
            graphics->fillRectangle(
                getBorderSize(),
                getBorderSize(),
                getWidth() - (2 * getBorderSize()),
                getHeight() - (2 * getBorderSize()));
        }
        // draw border or frame
        if (getBorderSize() > 0) {
            if (active && (getSelectionMode() & Widget::SelectionMode::Border) == Widget::SelectionMode::Border) {
                drawSelectionFrame(graphics);
            } else {
                drawBorder(graphics);
            }
        }
        if (mBarImage != nullptr) {
            if (getOrientation() == Orientation::Horizontal) {
                Rectangle const rec = Rectangle(
                    getBorderSize() + getPaddingLeft(),
                    getBorderSize() + getPaddingTop(),
                    mBarImage->getWidth() * mValue / mMaxValue,
                    mBarImage->getHeight());
                graphics->pushClipArea(rec);
                graphics->drawImage(mBarImage, 0, 0);
                graphics->popClipArea();
            } else {
                Rectangle const rec = Rectangle(
                    getBorderSize() + getPaddingLeft(),
                    getBorderSize() + getPaddingTop() +
                        (mBarImage->getHeight() - mBarImage->getHeight() * mValue / mMaxValue),
                    mBarImage->getWidth(),
                    mBarImage->getHeight() * mValue / mMaxValue);
                graphics->pushClipArea(rec);
                graphics->drawImage(mBarImage, 0, 0);
                graphics->popClipArea();
            }
        }

        if (mForegroundImage != nullptr) {
            int const x = getBorderSize() + getPaddingLeft();
            int const y = getBorderSize() + getPaddingTop();
            graphics->drawImage(mForegroundImage, x, y);
        }
    }

    void ImageProgressBar::setOpaque(bool opaque)
    {
        mOpaque = opaque;
    }

    bool ImageProgressBar::isOpaque() const
    {
        return mOpaque;
    }

    void ImageProgressBar::setBarImage(Image* image)
    {
        if (mInternalImage) {
            delete mBarImage;
        }

        mInternalImage = false;
        mBarImage      = image;

        adjustSize();
    }

    Image const * ImageProgressBar::getBarImage() const
    {
        return mBarImage;
    }

    void ImageProgressBar::setForegroundImage(Image* image)
    {
        mForegroundImage = image;
        adjustSize();
    }

    Image const * ImageProgressBar::getForegroundImage() const
    {
        return mForegroundImage;
    }

    void ImageProgressBar::setOrientation(Orientation orientation)
    {
        if (mOrientation != orientation) {
            if (orientation != Orientation::Horizontal && orientation != Orientation::Vertical) {
                fcn::throwException(
                    "Unknown orientation type in ImageProgressBar object",
                    static_cast<char const *>(__FUNCTION__),
                    __FILE__,
                    __LINE__);
                return;
            }
            mOrientation = orientation;
        }
    }

    ImageProgressBar::Orientation ImageProgressBar::getOrientation() const
    {
        return mOrientation;
    }

    void ImageProgressBar::setMaxValue(int value)
    {
        mMaxValue = value;
    }

    int ImageProgressBar::getMaxValue() const
    {
        return mMaxValue;
    }

    void ImageProgressBar::setValue(int value)
    {
        if (value > mMaxValue) {
            mValue = mMaxValue;
        } else if (value < 0) {
            mValue = 0;
        } else {
            mValue = value;
        }
    }

    int ImageProgressBar::getValue() const
    {
        return mValue;
    }

    void ImageProgressBar::resizeToContent(bool recursion)
    {
        adjustSize();
    }

    void ImageProgressBar::adjustSize()
    {
        int w = 0;
        int h = 0;
        if (mBarImage != nullptr) {
            w = mBarImage->getWidth();
            h = mBarImage->getHeight();
        }
        w += 2 * getBorderSize() + getPaddingLeft() + getPaddingRight();
        h += 2 * getBorderSize() + getPaddingTop() + getPaddingBottom();
        setSize(w, h);
    }
}; // namespace fcn
