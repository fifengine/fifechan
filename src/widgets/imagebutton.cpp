// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#include "fifechan/widgets/imagebutton.hpp"
#include <vector>
#include <string>

#include <algorithm>

#include "fifechan/exception.hpp"
#include "fifechan/font.hpp"
#include "fifechan/graphics.hpp"
#include "fifechan/image.hpp"

namespace fcn
{
    ImageButton::ImageButton()
    {
        mImages         = std::vector<Image const *>(6, static_cast<Image const *>(nullptr));
        mInternalImages = std::vector<bool>(6, false);
        adjustSize();
    }

    ImageButton::ImageButton(std::string const & filename)
    {
        mImages         = std::vector<Image const *>(6, static_cast<Image const *>(nullptr));
        mInternalImages = std::vector<bool>(6, false);
        setUpImage(filename);
    }

    ImageButton::ImageButton(Image const * image)
    {
        mImages         = std::vector<Image const *>(6, static_cast<Image const *>(nullptr));
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
        if (mInternalImages[static_cast<size_t>(type)]) {
            delete mImages[static_cast<size_t>(type)];
        }
        Image const * image = Image::load(filename);
        if (image != nullptr) {
            mImages[static_cast<size_t>(type)]         = image;
            mInternalImages[static_cast<size_t>(type)] = true;
        } else {
            mImages[static_cast<size_t>(type)]         = nullptr;
            mInternalImages[static_cast<size_t>(type)] = false;
        }
        adjustSize();
    }

    void ImageButton::setImage(Image const * image, ImageType type)
    {
        if (mInternalImages[static_cast<size_t>(type)]) {
            delete mImages[static_cast<size_t>(type)];
        }
        mImages[static_cast<size_t>(type)]         = image;
        mInternalImages[static_cast<size_t>(type)] = false;
        adjustSize();
    }

    void ImageButton::setUpImage(std::string const & filename)
    {
        setImage(filename, ImageType::Up);
    }

    void ImageButton::setUpImage(Image const * image)
    {
        setImage(image, ImageType::Up);
    }

    Image const * ImageButton::getUpImage() const
    {
        return mImages[static_cast<size_t>(ImageType::Up)];
    }

    void ImageButton::setDownImage(std::string const & filename)
    {
        setImage(filename, ImageType::Down);
    }

    void ImageButton::setDownImage(Image const * image)
    {
        setImage(image, ImageType::Down);
    }

    Image const * ImageButton::getDownImage() const
    {
        return mImages[static_cast<size_t>(ImageType::Down)];
    }

    void ImageButton::setHoverImage(std::string const & filename)
    {
        setImage(filename, ImageType::Hover);
    }

    void ImageButton::setHoverImage(Image const * image)
    {
        setImage(image, ImageType::Hover);
    }

    Image const * ImageButton::getHoverImage() const
    {
        return mImages[static_cast<size_t>(ImageType::Hover)];
    }

    void ImageButton::setInactiveUpImage(std::string const & filename)
    {
        setImage(filename, ImageType::Up_Inactive);
    }

    void ImageButton::setInactiveUpImage(Image const * image)
    {
        setImage(image, ImageType::Up_Inactive);
    }

    Image const * ImageButton::getInactiveUpImage() const
    {
        return mImages[static_cast<size_t>(ImageType::Up_Inactive)];
    }

    void ImageButton::setInactiveDownImage(std::string const & filename)
    {
        setImage(filename, ImageType::Down_Inactive);
    }

    void ImageButton::setInactiveDownImage(Image const * image)
    {
        setImage(image, ImageType::Down_Inactive);
    }

    Image const * ImageButton::getInactiveDownImage() const
    {
        return mImages[static_cast<size_t>(ImageType::Down_Inactive)];
    }

    void ImageButton::setInactiveHoverImage(std::string const & filename)
    {
        setImage(filename, ImageType::Hover_Inactive);
    }

    void ImageButton::setInactiveHoverImage(Image const * image)
    {
        setImage(image, ImageType::Hover_Inactive);
    }

    Image const * ImageButton::getInactiveHoverImage() const
    {
        return mImages[static_cast<size_t>(ImageType::Hover_Inactive)];
    }

    void ImageButton::resizeToContent(bool /*recursion*/)
    {
        adjustSize();
    }

    void ImageButton::adjustSize()
    {
        int w = 0;
        int h = 0;
        for (unsigned int i = 0; i < 6; ++i) {
            Image const * img = mImages[i];
            if (img != nullptr) {
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
            if (isFocused() && (getSelectionMode() & Widget::SelectionMode::Border) == Widget::SelectionMode::Border) {
                drawSelectionFrame(graphics);
            } else {
                drawBorder(graphics);
            }
        }

        Rectangle offsetRec(getBorderSize(), getBorderSize(), 2 * getBorderSize(), 2 * getBorderSize());
        // fetch the image, down, hover or up
        Image const * img = !isActive() && (getInactiveUpImage() != nullptr) ? getInactiveUpImage() : getUpImage();
        if (isPressed()) {
            offsetRec.x += getDownXOffset();
            offsetRec.y += getDownYOffset();
            if (!isActive()) {
                if (getInactiveDownImage() != nullptr) {
                    img = getInactiveDownImage();
                }
            } else {
                img = (getDownImage() != nullptr) ? getDownImage() : getUpImage();
            }
        } else if (mHasMouse) {
            if (!isActive()) {
                if (getInactiveHoverImage() != nullptr) {
                    img = getInactiveHoverImage();
                }
            } else {
                img = (getHoverImage() != nullptr) ? getHoverImage() : getUpImage();
            }
        }
        // render foreground image or color rectangle
        if (img != nullptr) {
            graphics->drawImage(
                img, 0, 0, offsetRec.x, offsetRec.y, getWidth() - offsetRec.width, getHeight() - offsetRec.height);
        } else {
            Color faceColor = getBaseColor();
            if (isFocused() &&
                ((getSelectionMode() & Widget::SelectionMode::Background) == Widget::SelectionMode::Background)) {
                faceColor = getSelectionColor();
            }
            int const alpha = faceColor.a;

            if (isPressed()) {
                faceColor   = faceColor - 0x303030;
                faceColor.a = alpha;
            }
            if (!isActive()) {
                int const color = static_cast<int>((faceColor.r * 0.3) + (faceColor.g * 0.59) + (faceColor.b * 0.11));
                faceColor.r     = color;
                faceColor.g     = color;
                faceColor.b     = color;
            }

            graphics->setColor(faceColor);
            graphics->fillRectangle(
                offsetRec.x, offsetRec.y, getWidth() - offsetRec.width, getHeight() - offsetRec.height);
        }

        // render caption if it exits
        if (!getCaption().empty()) {
            int textX = 0;
            int const textY =
                offsetRec.y + getPaddingTop() +
                ((getHeight() - offsetRec.height - getPaddingTop() - getPaddingBottom() - getFont()->getHeight()) / 2);
            switch (getAlignment()) {
            case Graphics::Alignment::Left:
                textX = offsetRec.x + getPaddingLeft();
                break;
            case Graphics::Alignment::Center:
                textX = offsetRec.x + getPaddingLeft() +
                        (getWidth() - offsetRec.width - getPaddingLeft() - getPaddingRight()) / 2;
                break;
            case Graphics::Alignment::Right:
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
