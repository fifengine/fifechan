// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#include "fifechan/backends/hge/hgeinput.hpp"

#include "fifechan/exception.hpp"

namespace fcn
{
    HGE* HGEInput::mHGE = NULL;

    HGEInput::HGEInput()
    {
        mHGE = hgeCreate(HGE_VERSION);

        mMouseX = 0;
        mMouseY = 0;

        mLeftMouseButtonDown   = false;
        mRightMouseButtonDown  = false;
        mMiddleMouseButtonDown = false;
    }

    HGEInput::~HGEInput()
    {
        mHGE->Release();
    }

    bool HGEInput::isKeyQueueEmpty()
    {
        return mKeyInputQueue.empty();
    }

    bool HGEInput::isMouseQueueEmpty()
    {
        return mMouseInputQueue.empty();
    }

    KeyInput HGEInput::dequeueKeyInput()
    {
        if (isKeyQueueEmpty()) {
            fcn::throwException(("Key queue is empty."), static_cast<char const *>(__FUNCTION__), __FILE__, __LINE__);
        }

        KeyInput keyInput;

        keyInput = mKeyInputQueue.front();
        mKeyInputQueue.pop();

        return keyInput;
    }

    MouseInput HGEInput::dequeueMouseInput()
    {
        if (isMouseQueueEmpty()) {
            fcn::throwException(("Mouse queue is empty."), static_cast<char const *>(__FUNCTION__), __FILE__, __LINE__);
        }

        MouseInput mouseInput;

        mouseInput = mMouseInputQueue.front();
        mMouseInputQueue.pop();

        return mouseInput;
    }

    void HGEInput::_pollInput()
    {
        hgeInputEvent ie;

        while (mHGE->Input_GetEvent(&ie)
            pollKeyInput(ie);

        pollMouseInput();
    }

    void HGEInput::pollMouseInput()
    {
        float curMouseX, curMouseY;
        int mouseWheel;

        bool leftBtn, rightBtn, centerBtn;

        mHGE->Input_GetMousePos(&curMouseX, &curMouseY);
        mouseWheel = mHGE->Input_GetMouseWheel();

        leftBtn   = mHGE->Input_GetKeyState(HGEK_LBUTTON);
        rightBtn  = mHGE->Input_GetKeyState(HGEK_RBUTTON);
        centerBtn = mHGE->Input_GetKeyState(HGEK_MBUTTON);

        // Check mouse movement
        if (mMouseX != curMouseX || mMouseY != curMouseY) {
            mMouseInputQueue.push(MouseInput(MouseInput::Empty, MouseInput::Moved, curMouseX, curMouseY, 0));

            mMouseX = curMouseX;
            mMouseY = curMouseY;
        }

        // Check mouse wheel
        if (mouseWheel > 0) {
            mMouseInputQueue.push(MouseInput(MouseInput::Empty, MouseInput::WheelMovedUp, curMouseX, curMouseY, 0));
        } else if (mouseWheel < 0) {
            mMouseInputQueue.push(MouseInput(MouseInput::Empty, MouseInput::WheelMovedDown, curMouseX, curMouseY, 0));
        }

        // check mouse buttons
        if (!mLeftMouseButtonDown && leftBtn) {
            mMouseInputQueue.push(MouseInput(MouseInput::Left, MouseInput::Pressed, curMouseX, curMouseY, 0));
        } else if (mLeftMouseButtonDown && !leftBtn) {
            mMouseInputQueue.push(MouseInput(MouseInput::Left, MouseInput::Released, curMouseX, curMouseY, 0));
        } else if (!mRightMouseButtonDown && rightBtn) {
            mMouseInputQueue.push(MouseInput(MouseInput::Right, MouseInput::Pressed, curMouseX, curMouseY, 0));
        } else if (mRightMouseButtonDown && !rightBtn) {
            mMouseInputQueue.push(MouseInput(MouseInput::Right, MouseInput::Released, curMouseX, curMouseY, 0));
        } else if (!mMiddleMouseButtonDown && centerBtn) {
            mMouseInputQueue.push(MouseInput(MouseInput::Middle, MouseInput::Pressed, curMouseX, curMouseY, 0));
        } else if (mMiddleMouseButtonDown && !centerBtn) {
            mMouseInputQueue.push(MouseInput(MouseInput::Middle, MouseInput::Released, curMouseX, curMouseY, 0));
        }

        mLeftMouseButtonDown   = leftBtn;
        mRightMouseButtonDown  = rightBtn;
        mMiddleMouseButtonDown = centerBtn;
    }

    void HGEInput::pollKeyInput(hgeInputEvent& ki)
    {
        if (ki.type != INPUT_KEYDOWN && ki.type != INPUT_KEYUP) {
            return;
        }

        if (ki.type == INPUT_KEYDOWN) {
            Key keyObj = convertToKey(ki.key, ki.chr);

            KeyInput keyInput(keyObj, KeyInput::Pressed);

            keyInput.setNumericPad(isNumericPad(ki.key));

            keyInput.setShiftPressed(ki.flags & HGEINP_SHIFT);
            keyInput.setAltPressed(ki.flags & HGEINP_ALT);
            keyInput.setControlPressed(ki.flags & HGEINP_CTRL);

            mKeyInputQueue.push(keyInput);
        }

        if (ki.type == INPUT_KEYUP) {
            Key keyObj = convertToKey(ki.key, ki.chr);

            KeyInput keyInput(keyObj, KeyInput::Released);

            keyInput.setNumericPad(isNumericPad(ki.key));

            keyInput.setShiftPressed(ki.flags & HGEINP_SHIFT);
            keyInput.setAltPressed(ki.flags & HGEINP_ALT);
            keyInput.setControlPressed(ki.flags & HGEINP_CTRL);

            mKeyInputQueue.push(keyInput);
        }
    }

    Key HGEInput::convertToKey(int key, int chr)
    {
        int keysym;

        switch (key) {
        case HGEK_TAB:
            keysym = Key::Tab;
            break;
        case HGEK_ESCAPE:
            keysym = Key::Escape;
            break;
        case HGEK_ALT:
            keysym = Key::LeftAlt;
            break;
        case HGEK_SHIFT:
            keysym = Key::LeftShift;
            break;
        case HGEK_CTRL:
            keysym = Key::LeftControl;
            break;
        case HGEK_LWIN:
            keysym = Key::LeftMeta;
            break;
        case HGEK_RWIN:
            keysym = Key::RightMeta;
            break;
        case HGEK_INSERT:
            keysym = Key::Insert;
            break;
        case HGEK_HOME:
            keysym = Key::Home;
            break;
        case HGEK_PGUP:
            keysym = Key::PageUp;
            break;
        case HGEK_PGDN:
            keysym = Key::PageDown;
            break;
        case HGEK_DELETE:
            keysym = Key::Delete;
            break;
        case HGEK_END:
            keysym = Key::End;
            break;
        case HGEK_CAPSLOCK:
            keysym = Key::CapsLock;
            break;
        case HGEK_BACKSPACE:
            keysym = Key::Backspace;
            break;
        case HGEK_F1:
            keysym = Key::F1;
            break;
        case HGEK_F2:
            keysym = Key::F2;
            break;
        case HGEK_F3:
            keysym = Key::F3;
            break;
        case HGEK_F4:
            keysym = Key::F4;
            break;
        case HGEK_F5:
            keysym = Key::F5;
            break;
        case HGEK_F6:
            keysym = Key::F6;
            break;
        case HGEK_F7:
            keysym = Key::F7;
            break;
        case HGEK_F8:
            keysym = Key::F8;
            break;
        case HGEK_F9:
            keysym = Key::F9;
            break;
        case HGEK_F10:
            keysym = Key::F10;
            break;
        case HGEK_F11:
            keysym = Key::F11;
            break;
        case HGEK_F12:
            keysym = Key::F12;
            break;
        case HGEK_PAUSE:
            keysym = Key::Pause;
            break;
        case HGEK_SCROLLLOCK:
            keysym = Key::ScrollLock;
            break;
        case HGEK_NUMLOCK:
            keysym = Key::NumLock;
            break;
        case HGEK_LEFT:
            keysym = Key::Left;
            break;
        case HGEK_RIGHT:
            keysym = Key::Right;
            break;
        case HGEK_UP:
            keysym = Key::Up;
            break;
        case HGEK_DOWN:
            keysym = Key::Down;
            break;
        case HGEK_ENTER:
            keysym = Key::Enter;
            break;
        default:
            keysym = chr;
        }

        Key k = Key(keysym);

        return k;
    }

    bool HGEInput::isNumericPad(int keyCode)
    {
        switch (keyCode) {
        case HGEK_NUMPAD0:
        case HGEK_NUMPAD1:
        case HGEK_NUMPAD2:
        case HGEK_NUMPAD3:
        case HGEK_NUMPAD4:
        case HGEK_NUMPAD5:
        case HGEK_NUMPAD6:
        case HGEK_NUMPAD7:
        case HGEK_NUMPAD8:
        case HGEK_NUMPAD9:
            return true;
        default:
            return false;
        }
    }
} // namespace fcn
