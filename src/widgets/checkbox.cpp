// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#include "fifechan/widgets/checkbox.hpp"

#include <algorithm>
#include <string>

#include "fifechan/exception.hpp"
#include "fifechan/font.hpp"
#include "fifechan/graphics.hpp"
#include "fifechan/image.hpp"

namespace fcn
{

    CheckBox::CheckBox()
    {

        adjustSize();
    }

    CheckBox::CheckBox(std::string const & caption, bool selected) : mSelected(selected)
    {

        setCaption(caption);
        adjustSize();
    }

    CheckBox::~CheckBox()
    {
        if (mInternalBackgroundImage) {
            delete mBackgroundImage;
        }
    }

    void CheckBox::draw(Graphics* graphics)
    {
        if (mMode == MarkerStyle::Rhombus) {
            drawRhombus(graphics);
        } else {
            drawBox(graphics);
        }

        if (!getCaption().empty()) {
            graphics->setFont(getFont());
            graphics->setColor(getForegroundColor());

            int const h     = getHeight() - (2 * getBorderSize()) - getPaddingTop() - getPaddingBottom();
            int const textX = getBorderSize() + getPaddingLeft() + h;
            int const textY = getBorderSize() + getPaddingTop() + ((h - getFont()->getHeight()) / 2);
            graphics->drawText(getCaption(), textX, textY);
        }
    }

    void CheckBox::drawBox(Graphics* graphics)
    {
        bool const active = isFocused();

        // draw background
        Rectangle const background(
            getBorderSize(), getBorderSize(), getWidth() - (2 * getBorderSize()), getHeight() - (2 * getBorderSize()));
        if (mBackgroundImage != nullptr) {
            graphics->drawImage(
                mBackgroundImage, 0, 0, background.x, background.y, background.width, background.height);
        } else {
            Color faceColor = getBaseColor();
            if (active &&
                ((getSelectionMode() & Widget::SelectionMode::Background) == Widget::SelectionMode::Background)) {
                faceColor = getSelectionColor();
            }
            graphics->setColor(faceColor);
            graphics->fillRectangle(background.x, background.y, background.width, background.height);
        }

        // rec for inner box
        int const h = getHeight() - (2 * getBorderSize()) - getPaddingTop() - getPaddingBottom();
        Rectangle const rec(getBorderSize() + getPaddingLeft(), getBorderSize() + getPaddingTop(), h, h);
        if (mMode == MarkerStyle::Image) {
            // draw marker image
            drawMarkerImage(graphics, rec);
            // draw border
            if (getBorderSize() > 0) {
                if (active && (getSelectionMode() & Widget::SelectionMode::Border) == Widget::SelectionMode::Border) {
                    drawSelectionFrame(graphics);
                } else {
                    drawBorder(graphics);
                }
            }
        } else {
            // draw inner box background
            graphics->setColor(getBackgroundColor());
            graphics->fillRectangle(rec.x, rec.y, h, h);
            // draw border
            if (getBorderSize() > 0) {
                if (active && (getSelectionMode() & Widget::SelectionMode::Border) == Widget::SelectionMode::Border) {
                    drawSelectionFrame(graphics);
                } else {
                    drawBorder(graphics);
                }
            }
            Color faceColor = getBaseColor();
            if (active &&
                ((getSelectionMode() & Widget::SelectionMode::Background) == Widget::SelectionMode::Background)) {
                faceColor = getSelectionColor();
            }
            // border around the inner box background
            int const alpha      = getBaseColor().a;
            faceColor.a          = alpha;
            Color highlightColor = faceColor + 0x303030;
            highlightColor.a     = alpha;
            Color shadowColor    = faceColor - 0x303030;
            shadowColor.a        = alpha;

            graphics->setColor(shadowColor);
            graphics->drawLine(rec.x, rec.y, h - 1, rec.y);
            graphics->drawLine(rec.x, rec.y, rec.x, h - 1);

            graphics->setColor(highlightColor);
            graphics->drawLine(h - 1, rec.x, h - 1, h - 1);
            graphics->drawLine(rec.y, h - 1, h - 1, h - 1);

            // draws marker
            if (mSelected) {
                graphics->setColor(getForegroundColor());
                switch (mMode) {
                case MarkerStyle::Checkmark:
                    drawCheckmark(graphics, rec);
                    break;
                case MarkerStyle::Cross:
                    drawCross(graphics, rec);
                    break;
                case MarkerStyle::Dot:
                    drawDot(graphics, rec);
                    break;
                default:
                    throwException("Unknown marker.");
                }
            }
        }
    }

    void CheckBox::drawCheckmark(Graphics* graphics, Rectangle const & rec)
    {
        graphics->drawLine(rec.x + 3, rec.y + 3, rec.x + 3, rec.height - 3);
        graphics->drawLine(rec.x + 4, rec.y + 4, rec.x + 4, rec.height - 2);
        graphics->drawLine(rec.x + 5, rec.height - 3, rec.width - 2, rec.y + 4);
        graphics->drawLine(rec.x + 5, rec.height - 4, rec.width - 4, rec.y + 5);
    }

    void CheckBox::drawCross(Graphics* graphics, Rectangle const & rec)
    {
        graphics->drawLine(rec.x + 2, rec.y + 2, rec.width - 3, rec.height - 3);
        graphics->drawLine(rec.x + 2, rec.y + 3, rec.width - 4, rec.height - 3);
        graphics->drawLine(rec.x + 2, rec.height - 3, rec.width - 3, rec.y + 2);
        graphics->drawLine(rec.x + 3, rec.height - 3, rec.width - 3, rec.y + 3);
    }

    void CheckBox::drawDot(Graphics* graphics, Rectangle const & rec)
    {
        const Point p(rec.x + (rec.width / 2), rec.y + (rec.height / 2));

        int const radius = (rec.width - 3) / 2;

        graphics->drawFillCircle(p, radius);
    }

    void CheckBox::drawMarkerImage(Graphics* graphics, Rectangle const & rec)
    {
        Rectangle rect    = rec;
        Image const * img = nullptr;
        if (isSelected()) {
            rect.x += getDownXOffset();
            rect.y += getDownYOffset();
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

        if (img != nullptr) {
            graphics->drawImage(img, 0, 0, rect.x, rect.y, std::max(rect.width, img->getWidth()), rect.height);
        }
    }

    void CheckBox::drawRhombus(Graphics* graphics)
    {
        // ToDo: Rewrite this part, its only c&p from RadioButton
        graphics->pushClipArea(Rectangle(1, 1, getWidth() - 1, getHeight() - 1));
        int h = 0;

        if (getHeight() % 2 == 0) {
            h = getHeight() - 4;
        } else {
            h = getHeight() - 3;
        }

        int const alpha      = getBaseColor().a;
        Color faceColor      = getBaseColor();
        faceColor.a          = alpha;
        Color highlightColor = faceColor + 0x303030;
        highlightColor.a     = alpha;
        Color shadowColor    = faceColor - 0x303030;
        shadowColor.a        = alpha;

        graphics->setColor(getBackgroundColor());

        int i        = 0;
        int const hh = (h + 1) / 2;

        for (i = 1; i <= hh; ++i) {
            graphics->drawLine(hh - i + 1, i, hh + i - 1, i);
        }

        for (i = 1; i < hh; ++i) {
            graphics->drawLine(hh - i + 1, h - i, hh + i - 1, h - i);
        }

        graphics->setColor(shadowColor);
        graphics->drawLine(hh, 0, 0, hh);
        graphics->drawLine(hh + 1, 1, h - 1, hh - 1);

        graphics->setColor(highlightColor);
        graphics->drawLine(1, hh + 1, hh, h);
        graphics->drawLine(hh + 1, h - 1, h, hh);

        graphics->setColor(getForegroundColor());

        int const hhh = hh - 3;

        if (mSelected) {
            for (i = 0; i < hhh; ++i) {
                graphics->drawLine(hh - i, 4 + i, hh + i, 4 + i);
            }
            for (i = 0; i < hhh; ++i) {
                graphics->drawLine(hh - i, h - 4 - i, hh + i, h - 4 - i);
            }
        }
        graphics->popClipArea();

        if (isFocused()) {
            int fh = 0;

            if (getHeight() % 2 == 0) {
                fh = getHeight() - 4;
            } else {
                fh = getHeight() - 3;
            }

            int const hh = (fh + 1) / 2;

            graphics->setColor(getSelectionColor());
            graphics->drawLine(0, hh + 1, hh + 1, 0);
            graphics->drawLine(hh + 2, 1, fh + 2, hh + 1);
            graphics->drawLine(fh + 1, hh + 2, hh + 1, fh + 2);
            graphics->drawLine(hh + 1, fh + 2, 1, hh + 2);
        }
    }

    bool CheckBox::isSelected() const
    {
        return mSelected;
    }

    void CheckBox::setSelected(bool selected)
    {
        mSelected = selected;
    }

    void CheckBox::toggleSelected()
    {
        mSelected = !mSelected;
    }

    void CheckBox::setBackgroundImage(std::string const & filename)
    {
        if (mInternalBackgroundImage) {
            delete mBackgroundImage;
        }
        mBackgroundImage         = Image::load(filename);
        mInternalBackgroundImage = true;
        adjustSize();
    }

    void CheckBox::setBackgroundImage(Image const * image)
    {
        if (mInternalBackgroundImage) {
            delete mBackgroundImage;
        }
        mBackgroundImage         = image;
        mInternalBackgroundImage = false;
        adjustSize();
    }

    Image const * CheckBox::getBackgroundImage() const
    {
        return mBackgroundImage;
    }

    CheckBox::MarkerStyle CheckBox::getMarkerStyle() const
    {
        return mMode;
    }

    void CheckBox::setMarkerStyle(CheckBox::MarkerStyle mode)
    {
        mMode = mode;
    }

    void CheckBox::keyPressed(KeyEvent& keyEvent)
    {
        Key const key = keyEvent.getKey();

        if (key.getValue() == Key::Enter || key.getValue() == Key::Space) {
            toggleSelected();
            keyEvent.consume();
            distributeActionEvent();
        }
    }

    void CheckBox::keyReleased(KeyEvent& keyEvent)
    {
        Key const key = keyEvent.getKey();

        if (key.getValue() == Key::Enter || key.getValue() == Key::Space) {
            keyEvent.consume();
        }
    }

    void CheckBox::mousePressed(MouseEvent& mouseEvent)
    {
        if (mHasMouse) {
            mouseEvent.consume();
        }
    }

    void CheckBox::mouseReleased(MouseEvent& mouseEvent)
    {
        if (mHasMouse) {
            mouseEvent.consume();
        }
    }

    void CheckBox::mouseClicked(MouseEvent& mouseEvent)
    {
        if (mHasMouse && mouseEvent.getButton() == MouseEvent::Button::Left) {
            toggleSelected();
            mouseEvent.consume();
            distributeActionEvent();
        }
    }

    void CheckBox::adjustSize()
    {
        int w = 0;
        int h = 0;
        if (mBackgroundImage != nullptr) {
            w += mBackgroundImage->getWidth() + 2 * getBorderSize();
            h += mBackgroundImage->getHeight() + 2 * getBorderSize();
        } else {
            if (!getCaption().empty()) {
                w = getFont()->getWidth(getCaption());
                h = getFont()->getHeight();
            }

            if (getUpImage() != nullptr) {
                w += getUpImage()->getWidth();
                h = std::max(getUpImage()->getHeight(), h);
            } else {
                // without image we need extra space for the mark
                w += h;
            }
            w += 2 * getBorderSize() + getPaddingLeft() + getPaddingRight();
            h += 2 * getBorderSize() + getPaddingTop() + getPaddingBottom();
        }
        setSize(w, h);
    }
} // namespace fcn
