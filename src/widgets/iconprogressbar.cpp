// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#include <fifechan/widgets/iconprogressbar.hpp>

#include <fifechan/exception.hpp>
#include <fifechan/graphics.hpp>
#include <fifechan/image.hpp>

namespace fcn
{
    IconProgressBar::IconProgressBar() :
        mImage(nullptr),
        mInternalImage(false),
        mMaxIcons(0),
        mIconCounter(0),
        mOrientation(Orientation::Horizontal),
        mOpaque(true)
    {
    }

    IconProgressBar::IconProgressBar(Image* image, int maxIcons) :
        mImage(image),
        mInternalImage(false),
        mMaxIcons(maxIcons),
        mIconCounter(0),
        mOrientation(Orientation::Horizontal),
        mOpaque(true)
    {
        adjustSize();
    }

    IconProgressBar::IconProgressBar(std::string const & filename, int maxIcons) :
        mImage(nullptr),
        mInternalImage(true),
        mMaxIcons(maxIcons),
        mIconCounter(0),
        mOrientation(Orientation::Horizontal),
        mOpaque(true)
    {
        mImage = Image::load(filename);
        adjustSize();
    }

    IconProgressBar::~IconProgressBar()
    {
        if (mInternalImage) {
            delete mImage;
        }
    }

    void IconProgressBar::draw(Graphics* graphics)
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
        if (mImage == nullptr) {
            return;
        }
        // draw "icons"
        int x = getBorderSize() + getPaddingLeft();
        int y = getBorderSize() + getPaddingTop();
        if (mOrientation == Orientation::Horizontal) {
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
        if (mInternalImage) {
            delete mImage;
        }

        mInternalImage = false;
        mImage         = image;

        adjustSize();
    }

    Image const * IconProgressBar::getImage() const
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
            if (orientation != Orientation::Horizontal && orientation != Orientation::Vertical) {
                fcn::throwException(
                    "Unknown orientation type in IconProgressBar object",
                    static_cast<char const *>(__FUNCTION__),
                    __FILE__,
                    __LINE__);
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
        mIconCounter = (mIconCounter + 1) % (mMaxIcons + 1);
    }

    void IconProgressBar::reset()
    {
        mIconCounter = 0;
    }

    void IconProgressBar::setIconCount(int icons)
    {
        mIconCounter = icons % (mMaxIcons + 1);
    }

    int IconProgressBar::getIconCount() const
    {
        return mIconCounter;
    }

    void IconProgressBar::resizeToContent(bool recursion)
    {
        adjustSize();
    }

    void IconProgressBar::adjustSize()
    {
        int w = 0;
        int h = 0;
        if (mImage != nullptr) {
            w = mImage->getWidth();
            h = mImage->getHeight();
            if (mOrientation == Orientation::Horizontal) {
                w *= mMaxIcons;
            } else {
                h *= mMaxIcons;
            }
        }
        w += 2 * getBorderSize() + getPaddingLeft() + getPaddingRight();
        h += 2 * getBorderSize() + getPaddingTop() + getPaddingBottom();
        setSize(w, h);
    }
}; // namespace fcn
