// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#include "fifechan/widgets/button.hpp"

#include <string>

#include "fifechan/exception.hpp"
#include "fifechan/font.hpp"
#include "fifechan/graphics.hpp"
#include "fifechan/key.hpp"
#include "fifechan/mouseevent.hpp"
#include "fifechan/mouseinput.hpp"

namespace fcn
{
    Button::Button() :
        mHasMouse(false),
        mKeyPressed(false),
        mMousePressed(false),
        mState(true),
        mAlignment(Graphics::Alignment::Center),
        mXOffset(1),
        mYOffset(1)
    {
        setFocusable(true);
        adjustSize();

        addMouseListener(this);
        addKeyListener(this);
        addFocusListener(this);
        addWidgetListener(this);
    }

    Button::Button(std::string const & caption) :
        mCaption(caption),
        mHasMouse(false),
        mKeyPressed(false),
        mMousePressed(false),
        mState(true),
        mAlignment(Graphics::Alignment::Center),
        mXOffset(1),
        mYOffset(1)
    {
        setFocusable(true);
        adjustSize();

        addMouseListener(this);
        addKeyListener(this);
        addFocusListener(this);
        addWidgetListener(this);
    }

    Button::~Button() { }

    void Button::setCaption(std::string const & caption)
    {
        mCaption = caption;
        adjustSize();
    }

    std::string const & Button::getCaption() const
    {
        return mCaption;
    }

    void Button::setActive(bool state)
    {
        mState = state;
    }

    bool Button::isActive() const
    {
        return mState;
    }

    void Button::setAlignment(Graphics::Alignment alignment)
    {
        mAlignment = alignment;
    }

    Graphics::Alignment Button::getAlignment() const
    {
        return mAlignment;
    }

    void Button::setDownXOffset(int offset)
    {
        mXOffset = offset;
    }

    int Button::getDownXOffset() const
    {
        return mXOffset;
    }

    void Button::setDownYOffset(int offset)
    {
        mYOffset = offset;
    }

    int Button::getDownYOffset() const
    {
        return mYOffset;
    }

    void Button::setDownOffset(int x, int y)
    {
        mXOffset = x;
        mYOffset = y;
    }

    void Button::fontChanged()
    {
        adjustSize();
    }

    void Button::draw(Graphics* graphics)
    {
        bool const active = isFocused();
        Color faceColor   = getBaseColor();
        if (active && ((getSelectionMode() & Widget::SelectionMode::Background) == Widget::SelectionMode::Background)) {
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
        Rectangle const offsetRec(getBorderSize(), getBorderSize(), 2 * getBorderSize(), 2 * getBorderSize());
        graphics->fillRectangle(offsetRec.x, offsetRec.y, getWidth() - offsetRec.width, getHeight() - offsetRec.height);

        if (getBorderSize() > 0) {
            if (active && (getSelectionMode() & Widget::SelectionMode::Border) == Widget::SelectionMode::Border) {
                drawSelectionFrame(graphics);
            } else {
                drawBorder(graphics);
            }
        }

        int textX = 0;
        int const textY =
            offsetRec.y + getPaddingTop() +
            (getHeight() - offsetRec.height - getPaddingTop() - getPaddingBottom() - getFont()->getHeight()) / 2;
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
            throwException("Unknown alignment.");
        }

        // set font and color
        graphics->setFont(getFont());
        graphics->setColor(getForegroundColor());
        if (isPressed()) {
            graphics->drawText(getCaption(), textX + getDownXOffset(), textY + getDownYOffset(), getAlignment());
        } else {
            graphics->drawText(getCaption(), textX, textY, getAlignment());
        }
    }

    void Button::resizeToContent(bool recursion)
    {
        adjustSize();
    }

    void Button::adjustSize()
    {
        int const w = getFont()->getWidth(mCaption) + (2 * getBorderSize()) + getPaddingLeft() + getPaddingRight();
        int const h = getFont()->getHeight() + (2 * getBorderSize()) + getPaddingTop() + getPaddingBottom();
        setSize(w, h);
    }

    bool Button::isPressed() const
    {
        if (mMousePressed) {
            return mHasMouse;
        }
        return mKeyPressed;
    }

    void Button::mousePressed(MouseEvent& mouseEvent)
    {
        if (mouseEvent.getButton() == MouseEvent::Button::Left) {
            mMousePressed = true;
            mouseEvent.consume();
        }
    }

    void Button::mouseExited(MouseEvent& mouseEvent)
    {
        mHasMouse = false;
    }

    void Button::mouseEntered(MouseEvent& mouseEvent)
    {
        mHasMouse = true;
    }

    void Button::mouseReleased(MouseEvent& mouseEvent)
    {
        if (mouseEvent.getButton() == MouseEvent::Button::Left && mMousePressed && mHasMouse) {
            mMousePressed = false;
            distributeActionEvent();
            mouseEvent.consume();
        } else if (mouseEvent.getButton() == MouseEvent::Button::Left) {
            mMousePressed = false;
            mouseEvent.consume();
        }
    }

    void Button::mouseDragged(MouseEvent& mouseEvent)
    {
        mouseEvent.consume();
    }

    void Button::keyPressed(KeyEvent& keyEvent)
    {
        Key const key = keyEvent.getKey();

        if (key.getValue() == Key::Enter || key.getValue() == Key::Space) {
            mKeyPressed = true;
            keyEvent.consume();
        }
    }

    void Button::keyReleased(KeyEvent& keyEvent)
    {
        Key const key = keyEvent.getKey();

        if ((key.getValue() == Key::Enter || key.getValue() == Key::Space) && mKeyPressed) {
            mKeyPressed = false;
            distributeActionEvent();
            keyEvent.consume();
        }
    }

    void Button::focusLost(Event const & event)
    {
        mMousePressed = false;
        mKeyPressed   = false;
        mHasMouse     = false;
    }

    void Button::ancestorHidden(Event const & e)
    {
        mMousePressed = false;
        mKeyPressed   = false;
        mHasMouse     = false;
    }
} // namespace fcn
