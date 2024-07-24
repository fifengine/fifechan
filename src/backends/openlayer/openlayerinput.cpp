// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#include "fifechan/backends/openlayer/openlayerinput.hpp"

#include <allegro.h>

#include "fifechan/exception.hpp"

namespace fcn
{
    OpenLayerInput::OpenLayerInput()
    {
        mMouseButton1 = mMouseButton2 = mMouseButton3 = false;
        mLastMouseZ                                   = 0;
        mLastMouseX                                   = 0;
        mLastMouseY                                   = 0;
    }

    bool OpenLayerInput::isKeyQueueEmpty()
    {
        return mKeyQueue.empty();
    }

    KeyInput OpenLayerInput::dequeueKeyInput()
    {
        if (isKeyQueueEmpty()) {
            fcn::throwException("Key queue is empty.", static_cast<char const *>(__FUNCTION__), __FILE__, __LINE__);
        }

        KeyInput ki = mKeyQueue.front();
        mKeyQueue.pop();

        return ki;
    }

    bool OpenLayerInput::isMouseQueueEmpty()
    {
        return mMouseQueue.empty();
    }

    MouseInput OpenLayerInput::dequeueMouseInput()
    {
        if (isMouseQueueEmpty()) {
            fcn::throwException("Mouse queue is empty.", static_cast<char const *>(__FUNCTION__), __FILE__, __LINE__);
        }

        MouseInput mi = mMouseQueue.front();
        mMouseQueue.pop();

        return mi;
    }

    void OpenLayerInput::_pollInput()
    {
        pollMouseInput();
        pollKeyInput();
    }

    void OpenLayerInput::pollMouseInput()
    {
        if (mouse_needs_poll()) {
            poll_mouse();
        }
        int mouseX  = mouse_x;
        int mouseY  = mouse_y;
        int mouseZ  = mouse_z;
        int mouseB1 = mouse_b & 1;
        int mouseB2 = mouse_b & 2;
        int mouseB3 = mouse_b & 4;

        // Check mouse movement
        if (mouseX != mLastMouseX || mouseY != mLastMouseY) {
            mMouseQueue.push(MouseInput(MouseInput::Empty, MouseInput::Moved, mouseX, mouseY, 0));
            mLastMouseX = mouseX;
            mLastMouseY = mouseY;
        }

        // Check mouse Wheel
        while (mLastMouseZ < mouseZ) {
            mMouseQueue.push(MouseInput(MouseInput::Empty, MouseInput::WheelMovedUp, mouseX, mouseY, 0));
            mLastMouseZ++;
        }

        while (mLastMouseZ > mouseZ) {
            mMouseQueue.push(MouseInput(MouseInput::Empty, MouseInput::WheelMovedDown, mouseX, mouseY, 0));
            mLastMouseZ--;
        }

        // Check mouse buttons
        if (!mMouseButton1 && mouseB1) {
            mMouseQueue.push(MouseInput(MouseInput::Left, MouseInput::Pressed, mouseX, mouseY, 0));
        }

        if (mMouseButton1 && !mouseB1) {
            mMouseQueue.push(MouseInput(MouseInput::Left, MouseInput::Released, mouseX, mouseY, 0));
        }

        if (!mMouseButton2 && mouseB2) {
            mMouseQueue.push(MouseInput(MouseInput::Right, MouseInput::Pressed, mouseX, mouseY, 0));
        }

        if (mMouseButton2 && !mouseB2) {
            mMouseQueue.push(MouseInput(MouseInput::Right, MouseInput::Released, mouseX, mouseY, 0));
        }

        if (!mMouseButton3 && mouseB3) {
            mMouseQueue.push(MouseInput(MouseInput::Middle, MouseInput::Pressed, mouseX, mouseY, 0));
        }

        if (mMouseButton3 && !mouseB3) {
            mMouseQueue.push(MouseInput(MouseInput::Middle, MouseInput::Released, mouseX, mouseY, 0));
        }

        mMouseButton1 = mouseB1;
        mMouseButton2 = mouseB2;
        mMouseButton3 = mouseB3;
    }

    void OpenLayerInput::pollKeyInput()
    {
        int unicode, scancode;

        if (keyboard_needs_poll()) {
            poll_keyboard();
        }

        while (keypressed()) {
            unicode    = ureadkey(&scancode);
            Key keyObj = convertToKey(scancode, unicode);

            KeyInput keyInput(keyObj, KeyInput::Pressed);

            keyInput.setNumericPad(isNumericPad(scancode));
            keyInput.setShiftPressed(key_shifts & KB_SHIFT_FLAG);
            keyInput.setAltPressed(key_shifts & KB_ALT_FLAG);
            keyInput.setControlPressed(key_shifts & KB_CTRL_FLAG);
#ifdef KB_COMMAND_FLAG
            keyInput.setMetaPressed(key_shifts & (KB_COMMAND_FLAG | KB_LWIN_FLAG | KB_RWIN_FLAG));
#else
            keyInput.setMetaPressed(key_shifts & (KB_LWIN_FLAG | KB_RWIN_FLAG));
#endif

            mKeyQueue.push(keyInput);

            mPressedKeys[scancode] = keyInput;
        }

        if (key[KEY_ALT] && mPressedKeys.find(KEY_ALT) == mPressedKeys.end()) {
            KeyInput keyInput(convertToKey(KEY_ALT, 0), KeyInput::Pressed);
            mKeyQueue.push(keyInput);
            mPressedKeys[KEY_ALT] = keyInput;
        }

        if (key[KEY_ALTGR] && mPressedKeys.find(KEY_ALTGR) == mPressedKeys.end()) {
            KeyInput keyInput(convertToKey(KEY_ALTGR, 0), KeyInput::Pressed);
            mKeyQueue.push(keyInput);
            mPressedKeys[KEY_ALTGR] = keyInput;
        }

        if (key[KEY_LSHIFT] && mPressedKeys.find(KEY_LSHIFT) == mPressedKeys.end()) {
            KeyInput keyInput(convertToKey(KEY_LSHIFT, 0), KeyInput::Pressed);
            mKeyQueue.push(keyInput);
            mPressedKeys[KEY_LSHIFT] = keyInput;
        }

        if (key[KEY_RSHIFT] && mPressedKeys.find(KEY_RSHIFT) == mPressedKeys.end()) {
            KeyInput keyInput(convertToKey(KEY_RSHIFT, 0), KeyInput::Pressed);
            mKeyQueue.push(keyInput);
            mPressedKeys[KEY_RSHIFT] = keyInput;
        }

        if (key[KEY_LCONTROL] && mPressedKeys.find(KEY_LCONTROL) == mPressedKeys.end()) {
            KeyInput keyInput(convertToKey(KEY_LCONTROL, 0), KeyInput::Pressed);
            mKeyQueue.push(keyInput);
            mPressedKeys[KEY_LCONTROL] = keyInput;
        }

        if (key[KEY_RCONTROL] && mPressedKeys.find(KEY_RCONTROL) == mPressedKeys.end()) {
            KeyInput keyInput(convertToKey(KEY_RCONTROL, 0), KeyInput::Pressed);
            mKeyQueue.push(keyInput);
            mPressedKeys[KEY_RCONTROL] = keyInput;
        }

        // Check for released keys
        std::map<int, KeyInput>::iterator iter, tempIter;
        for (iter = mPressedKeys.begin(); iter != mPressedKeys.end();) {
            if (!key[iter->first]) {
                KeyInput keyInput(iter->second.getKey(), KeyInput::Released);
                keyInput.setNumericPad(iter->second.isNumericPad());
                keyInput.setShiftPressed(iter->second.isShiftPressed());
                keyInput.setAltPressed(iter->second.isAltPressed());
                keyInput.setControlPressed(iter->second.isControlPressed());

                mKeyQueue.push(keyInput);

                tempIter = iter;
                iter++;
                mPressedKeys.erase(tempIter);
            } else {
                iter++;
            }
        }
    }

    Key OpenLayerInput::convertToKey(int scancode, int unicode)
    {
        int keysym;
        bool pad = false;

        switch (scancode) {
        case KEY_ESC:
            keysym = Key::Escape;
            break;

        case KEY_ALT:
            keysym = Key::LeftAlt;
            break;

        case KEY_ALTGR:
            keysym = Key::RightAlt;
            break;

        case KEY_LSHIFT:
            keysym = Key::LeftShift;
            break;

        case KEY_RSHIFT:
            keysym = Key::RightShift;
            break;

        case KEY_LCONTROL:
            keysym = Key::LeftControl;
            break;

        case KEY_RCONTROL:
            keysym = Key::RightControl;
            break;

        case KEY_LWIN:
            keysym = Key::LeftMeta;
            break;

        case KEY_RWIN:
            keysym = Key::RightMeta;
            break;

        case KEY_INSERT:
            keysym = Key::Insert;
            break;

        case KEY_HOME:
            keysym = Key::Home;
            break;

        case KEY_PGUP:
            keysym = Key::PageUp;
            break;

        case KEY_PGDN:
            keysym = Key::PageDown;
            break;

        case KEY_DEL:
            keysym = Key::Delete;
            break;

        case KEY_DEL_PAD:
            keysym = Key::Delete;
            pad    = true;
            break;

        case KEY_END:
            keysym = Key::End;
            break;

        case KEY_CAPSLOCK:
            keysym = Key::CapsLock;
            break;

        case KEY_BACKSPACE:
            keysym = Key::Backspace;
            break;

        case KEY_F1:
            keysym = Key::F1;
            break;

        case KEY_F2:
            keysym = Key::F2;
            break;

        case KEY_F3:
            keysym = Key::F3;
            break;

        case KEY_F4:
            keysym = Key::F4;
            break;

        case KEY_F5:
            keysym = Key::F5;
            break;

        case KEY_F6:
            keysym = Key::F6;
            break;

        case KEY_F7:
            keysym = Key::F7;
            break;

        case KEY_F8:
            keysym = Key::F8;
            break;

        case KEY_F9:
            keysym = Key::F9;
            break;

        case KEY_F10:
            keysym = Key::F10;
            break;

        case KEY_F11:
            keysym = Key::F11;
            break;

        case KEY_F12:
            keysym = Key::F12;
            break;

        case KEY_PRTSCR:
            keysym = Key::PrintScreen;
            break;

        case KEY_PAUSE:
            keysym = Key::Pause;
            break;

        case KEY_SCRLOCK:
            keysym = Key::ScrollLock;
            break;

        case KEY_NUMLOCK:
            keysym = Key::NumLock;
            break;

        case KEY_LEFT:
            keysym = Key::Left;
            break;

        case KEY_RIGHT:
            keysym = Key::Right;
            break;

        case KEY_UP:
            keysym = Key::Up;
            break;

        case KEY_DOWN:
            keysym = Key::Down;
            break;

        case KEY_ENTER_PAD:
            pad = true;
        case KEY_ENTER:
            keysym = Key::Enter;
            break;
        default:
            keysym = unicode;
        }

        Key k = Key(keysym);

        return k;
    }

    bool OpenLayerInput::isNumericPad(int scancode)
    {
        switch (scancode) {
        case KEY_0_PAD:
        case KEY_1_PAD:
        case KEY_2_PAD:
        case KEY_3_PAD:
        case KEY_4_PAD:
        case KEY_5_PAD:
        case KEY_6_PAD:
        case KEY_7_PAD:
        case KEY_8_PAD:
        case KEY_9_PAD:
        case KEY_SLASH_PAD:
        case KEY_MINUS_PAD:
        case KEY_PLUS_PAD:
            return true;
        default:
            return false;
        }
    }
} // namespace fcn
