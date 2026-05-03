// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

// Corresponding header include
#include "fifechan/mouseinput.hpp"

// Standard library includes
#include <cassert>

namespace fcn
{
    MouseInput::MouseInput(Button button, Type type, int x, int y, int timeStamp) :
        mType(type), mButton(button), mTimeStamp(timeStamp), mX(x), mY(y)
    {
        assert(
            "Type must be valid" &&
            (type == Type::Pressed || type == Type::Released || type == Type::Moved || type == Type::WheelMovedUp ||
             type == Type::WheelMovedDown || type == Type::WheelMovedRight || type == Type::WheelMovedLeft));
        assert(
            "Button must be valid" && (button == Button::Empty || button == Button::Left || button == Button::Right ||
                                       button == Button::Middle || button == Button::X1 || button == Button::X2));
        assert("Time stamp must be non-negative" && timeStamp >= 0);
    }

    void MouseInput::setType(Type type)
    {
        assert(
            "Type must be valid" &&
            (type == Type::Pressed || type == Type::Released || type == Type::Moved || type == Type::WheelMovedUp ||
             type == Type::WheelMovedDown || type == Type::WheelMovedRight || type == Type::WheelMovedLeft));
        mType = type;
    }

    MouseInput::Type MouseInput::getType() const
    {
        return mType;
    }

    void MouseInput::setButton(Button button)
    {
        assert(
            "Button must be valid" && (button == Button::Empty || button == Button::Left || button == Button::Right ||
                                       button == Button::Middle || button == Button::X1 || button == Button::X2));
        mButton = button;
    }

    MouseInput::Button MouseInput::getButton() const
    {
        return mButton;
    }

    int MouseInput::getTimeStamp() const
    {
        return mTimeStamp;
    }

    void MouseInput::setTimeStamp(int timeStamp)
    {
        assert("Time stamp must be non-negative" && timeStamp >= 0);
        mTimeStamp = timeStamp;
    }

    void MouseInput::setX(int x)
    {
        mX = x;
    }

    int MouseInput::getX() const
    {
        return mX;
    }

    void MouseInput::setY(int y)
    {
        mY = y;
    }

    int MouseInput::getY() const
    {
        return mY;
    }
} // namespace fcn
