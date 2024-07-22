// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#include "fifechan/genericinput.hpp"

#include "fifechan/exception.hpp"
#include "fifechan/keyinput.hpp"
#include "fifechan/mouseinput.hpp"

namespace fcn
{
    GenericInput::GenericInput() = default;

    void GenericInput::pushKeyPressed(int unicode)
    {
        // TODO
    }

    void GenericInput::pushKeyReleased(int unicode)
    {
        // TODO
    }

    void GenericInput::pushMouseButtonPressed(int x, int y, int button)
    {
        MouseInput mouseInput{};
        mouseInput.setX(x);
        mouseInput.setY(y);
        mouseInput.setButton(button);
        mouseInput.setType(MouseInput::Pressed);

        mMouseInputQueue.push(mouseInput);
    }

    void GenericInput::pushMouseButtonReleased(int x, int y, int button)
    {
        MouseInput mouseInput{};
        mouseInput.setX(x);
        mouseInput.setY(y);
        mouseInput.setButton(button);
        mouseInput.setType(MouseInput::Released);

        mMouseInputQueue.push(mouseInput);
    }

    void GenericInput::pushMouseWheelMovedUp(int x, int y)
    {
        MouseInput mouseInput{};
        mouseInput.setX(x);
        mouseInput.setY(y);
        mouseInput.setType(MouseInput::WheelMovedUp);

        mMouseInputQueue.push(mouseInput);
    }

    void GenericInput::pushMouseWheelMovedDown(int x, int y)
    {
        MouseInput mouseInput{};
        mouseInput.setX(x);
        mouseInput.setY(y);
        mouseInput.setType(MouseInput::WheelMovedDown);

        mMouseInputQueue.push(mouseInput);
    }

    void GenericInput::pushMouseWheelMovedRight(int x, int y)
    {
        MouseInput mouseInput{};
        mouseInput.setX(x);
        mouseInput.setY(y);
        mouseInput.setType(MouseInput::WheelMovedRight);

        mMouseInputQueue.push(mouseInput);
    }

    void GenericInput::pushMouseWheelMovedLeft(int x, int y)
    {
        MouseInput mouseInput{};
        mouseInput.setX(x);
        mouseInput.setY(y);
        mouseInput.setType(MouseInput::WheelMovedLeft);

        mMouseInputQueue.push(mouseInput);
    }

    void GenericInput::pushMouseMoved(int x, int y)
    {
        MouseInput mouseInput{};
        mouseInput.setX(x);
        mouseInput.setY(y);
        mouseInput.setType(MouseInput::Moved);

        mMouseInputQueue.push(mouseInput);
    }

    bool GenericInput::isKeyQueueEmpty()
    {
        return mKeyInputQueue.empty();
    }

    KeyInput GenericInput::dequeueKeyInput()
    {
        KeyInput keyInput;

        if (mKeyInputQueue.empty()) {
            fcn::throwException("The queue is empty.", static_cast<char const *>(__FUNCTION__), __FILE__, __LINE__);
        }

        keyInput = mKeyInputQueue.front();
        mKeyInputQueue.pop();

        return keyInput;
    }

    bool GenericInput::isMouseQueueEmpty()
    {
        return mMouseInputQueue.empty();
    }

    MouseInput GenericInput::dequeueMouseInput()
    {
        MouseInput mouseInput{};

        if (mMouseInputQueue.empty()) {
            fcn::throwException("The queue is empty.", static_cast<char const *>(__FUNCTION__), __FILE__, __LINE__);
        }

        mouseInput = mMouseInputQueue.front();
        mMouseInputQueue.pop();

        return mouseInput;
    }

    void GenericInput::_pollInput()
    {
        // Does nothing.
    }
} // namespace fcn
