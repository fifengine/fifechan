// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#include "fifechan/mouseinput.hpp"

namespace fcn
{
    MouseInput::MouseInput(Button button, Type type, int x, int y, int timeStamp) :
        mType(type), mButton(button), mTimeStamp(timeStamp), mX(x), mY(y)
    {
    }

    void MouseInput::setType(Type type)
    {
        mType = type;
    }

    MouseInput::Type MouseInput::getType() const
    {
        return mType;
    }

    void MouseInput::setButton(Button button)
    {
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
