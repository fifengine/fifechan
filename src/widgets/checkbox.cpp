// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#include <algorithm>

#include "fifechan/exception.hpp"
#include "fifechan/font.hpp"
#include "fifechan/graphics.hpp"
#include "fifechan/image.hpp"

#include "fifechan/widgets/checkbox.hpp"

namespace fcn
{

    CheckBox::CheckBox() :
        mBackgroundImage(0), mInternalBackgroundImage(false), mSelected(false), mMode(Marker_Checkmark)
    {

        adjustSize();
    }

    CheckBox::CheckBox(std::string const & caption, bool selected) :
        mBackgroundImage(0), mInternalBackgroundImage(false), mSelected(selected), mMode(Marker_Checkmark)
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
        if (mMode == Marker_Rhombus) {
            drawRhombus(graphics);
        } else {
            drawBox(graphics);
        }

        if (!getCaption().empty()) {
            graphics->setFont(getFont());
            graphics->setColor(getForegroundColor());

            int h     = getHeight() - 2 * getBorderSize() - getPaddingTop() - getPaddingBottom();
            int textX = getBorderSize() + getPaddingLeft() + h;
            int textY = getBorderSize() + getPaddingTop() + (h - getFont()->getHeight()) / 2;
            graphics->drawText(getCaption(), textX, textY);
        }
    }

    void CheckBox::drawBox(Graphics* graphics)
    {
        bool active = isFocused();

        // draw background
        Rectangle background(
            getBorderSize(), getBorderSize(), getWidth() - 2 * getBorderSize(), getHeight() - 2 * getBorderSize());
        if (mBackgroundImage) {
            graphics->drawImage(
                mBackgroundImage, 0, 0, background.x, background.y, background.width, background.height);
        } else {
            Color faceColor = getBaseColor();
            if (active && ((getSelectionMode() & Widget::Selection_Background) == Widget::Selection_Background)) {
                faceColor = getSelectionColor();
            }
            graphics->setColor(faceColor);
            graphics->fillRectangle(background.x, background.y, background.width, background.height);
        }

        // rec for inner box
        int h = getHeight() - 2 * getBorderSize() - getPaddingTop() - getPaddingBottom();
        Rectangle rec(getBorderSize() + getPaddingLeft(), getBorderSize() + getPaddingTop(), h, h);
        if (mMode == Marker_Image) {
            // draw marker image
            drawMarkerImage(graphics, rec);
            // draw border
            if (getBorderSize() > 0) {
                if (active && (getSelectionMode() & Widget::Selection_Border) == Widget::Selection_Border) {
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
                if (active && (getSelectionMode() & Widget::Selection_Border) == Widget::Selection_Border) {
                    drawSelectionFrame(graphics);
                } else {
                    drawBorder(graphics);
                }
            }
            Color faceColor = getBaseColor();
            if (active && ((getSelectionMode() & Widget::Selection_Background) == Widget::Selection_Background)) {
                faceColor = getSelectionColor();
            }
            // border around the inner box background
            int alpha            = getBaseColor().a;
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
                case Marker_Checkmark:
                    drawCheckmark(graphics, rec);
                    break;
                case Marker_Cross:
                    drawCross(graphics, rec);
                    break;
                case Marker_Dot:
                    drawDot(graphics, rec);
                    break;
                default:
                    fcn::throwException("Unknown marker.", static_cast<char const *>(__FUNCTION__), __FILE__, __LINE__);
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
        Point p(rec.x + rec.width / 2, rec.y + rec.height / 2);
        graphics->drawFillCircle(p, (rec.width - 3) / 2);
    }

    void CheckBox::drawMarkerImage(Graphics* graphics, Rectangle const & rec)
    {
        Rectangle rect    = rec;
        Image const * img = 0;
        if (isSelected()) {
            rect.x += getDownXOffset();
            rect.y += getDownYOffset();
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

        if (img) {
            graphics->drawImage(img, 0, 0, rect.x, rect.y, std::max(rect.width, img->getWidth()), rect.height);
        }
    }

    void CheckBox::drawRhombus(Graphics* graphics)
    {
        // ToDo: Rewrite this part, its only c&p from RadioButton
        graphics->pushClipArea(Rectangle(1, 1, getWidth() - 1, getHeight() - 1));
        int h;

        if (getHeight() % 2 == 0) {
            h = getHeight() - 4;
        } else {
            h = getHeight() - 3;
        }

        int alpha            = getBaseColor().a;
        Color faceColor      = getBaseColor();
        faceColor.a          = alpha;
        Color highlightColor = faceColor + 0x303030;
        highlightColor.a     = alpha;
        Color shadowColor    = faceColor - 0x303030;
        shadowColor.a        = alpha;

        graphics->setColor(getBackgroundColor());

        int i;
        int hh = (h + 1) / 2;

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

        int hhh = hh - 3;
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
            int fh;

            if (getHeight() % 2 == 0) {
                fh = getHeight() - 4;
            } else {
                fh = getHeight() - 3;
            }

            int hh = (fh + 1) / 2;

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
        Key key = keyEvent.getKey();
        if (key.getValue() == Key::Enter || key.getValue() == Key::Space) {
            toggleSelected();
            keyEvent.consume();
            distributeActionEvent();
        }
    }

    void CheckBox::keyReleased(KeyEvent& keyEvent)
    {
        Key key = keyEvent.getKey();
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
        if (mHasMouse && mouseEvent.getButton() == MouseEvent::Left) {
            toggleSelected();
            mouseEvent.consume();
            distributeActionEvent();
        }
    }

    void CheckBox::adjustSize()
    {
        int w = 0;
        int h = 0;
        if (mBackgroundImage) {
            w += mBackgroundImage->getWidth() + 2 * getBorderSize();
            h += mBackgroundImage->getHeight() + 2 * getBorderSize();
        } else {
            if (!getCaption().empty()) {
                w = getFont()->getWidth(getCaption());
                h = getFont()->getHeight();
            }

            if (getUpImage()) {
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
