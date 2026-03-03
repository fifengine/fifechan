// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#include "fifechan/widgets/icon.hpp"

#include "fifechan/graphics.hpp"
#include "fifechan/image.hpp"
#include "fifechan/rectangle.hpp"

namespace fcn
{
    Icon::Icon() : mImage(nullptr), mInternalImage(false), mScale(false), mTile(false), mOpaque(true)
    {

        adjustSize();
    }

    Icon::Icon(std::string const & filename) :
        mImage(nullptr), mInternalImage(false), mScale(false), mTile(false), mOpaque(true)
    {
        mImage         = Image::load(filename);
        mInternalImage = true;
        adjustSize();
    }

    Icon::Icon(Image const * image) : mImage(image), mInternalImage(false), mScale(false), mTile(false), mOpaque(true)
    {

        adjustSize();
    }

    Icon::~Icon()
    {
        if (mInternalImage) {
            delete mImage;
        }
    }

    void Icon::setImage(std::string const & filename)
    {
        if (mInternalImage) {
            delete mImage;
        }

        mImage         = Image::load(filename);
        mInternalImage = true;
        adjustSize();
    }

    void Icon::setImage(Image const * image)
    {
        if (mInternalImage) {
            delete mImage;
        }

        mImage         = image;
        mInternalImage = false;
        adjustSize();
    }

    Image const * Icon::getImage() const
    {
        return mImage;
    }

    bool Icon::isScaling() const
    {
        return mScale;
    }

    void Icon::setScaling(bool scale)
    {
        mScale = scale;
    }

    bool Icon::isTiling() const
    {
        return mTile;
    }

    void Icon::setTiling(bool tile)
    {
        mTile = tile;
    }

    void Icon::setOpaque(bool opaque)
    {
        mOpaque = opaque;
    }

    bool Icon::isOpaque() const
    {
        return mOpaque;
    }

    void Icon::resizeToContent(bool recursion)
    {
        adjustSize();
    }

    void Icon::adjustSize()
    {
        // workaround to avoid resizing
        if (mScale || mTile) {
            return;
        }
        int w = (2 * getBorderSize()) + getPaddingLeft() + getPaddingRight();
        int h = (2 * getBorderSize()) + getPaddingTop() + getPaddingBottom();
        if (mImage != nullptr) {
            w += mImage->getWidth();
            h += mImage->getHeight();
        }
        setSize(w, h);
    }

    void Icon::draw(Graphics* graphics)
    {
        // draw icon background
        if (mOpaque) {
            Color color = getBackgroundColor();
            if (isFocused() &&
                ((getSelectionMode() & Widget::SelectionMode::Background) == Widget::SelectionMode::Background)) {
                color = getSelectionColor();
            }
            graphics->setColor(color);
            graphics->fillRectangle(Rectangle(
                getBorderSize(),
                getBorderSize(),
                getWidth() - (2 * getBorderSize()),
                getHeight() - (2 * getBorderSize())));
        }
        // draw icon image
        if (mImage != nullptr) {
            Rectangle const contentRect(
                getBorderSize() + getPaddingLeft(),
                getBorderSize() + getPaddingTop(),
                getWidth() - (2 * getBorderSize()) - getPaddingLeft() - getPaddingRight(),
                getHeight() - (2 * getBorderSize()) - getPaddingTop() - getPaddingBottom());

            // draw with widget or image size
            int const w = mScale ? contentRect.width : mImage->getWidth();
            int const h = mScale ? contentRect.height : mImage->getHeight();

            if (mTile && !mScale) {
                Rectangle rect(contentRect.x, contentRect.y, w, h);
                int const tmpW = getWidth() - getBorderSize() - getPaddingRight();
                int const tmpH = getHeight() - getBorderSize() - getPaddingBottom();
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
            if (isFocused() && (getSelectionMode() & Widget::SelectionMode::Border) == Widget::SelectionMode::Border) {
                drawSelectionFrame(graphics);
            } else {
                drawBorder(graphics);
            }
        }
    }
} // namespace fcn
