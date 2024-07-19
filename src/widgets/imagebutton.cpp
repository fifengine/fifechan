// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

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
                fcn::throwException("Unknown alignment.", static_cast<char const *>(__FUNCTION__), __FILE__, __LINE__);
            }
            // set font and color
            graphics->setFont(getFont());
            graphics->setColor(getForegroundColor());
            graphics->drawText(getCaption(), textX, textY, getAlignment());
        }
    }
} // namespace fcn
