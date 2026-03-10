// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#include "fifechan/keyinput.hpp"

#include "fifechan/key.hpp"

namespace fcn
{
    KeyInput::KeyInput(Key const & key, unsigned int type) : mKey(key), mType(type) { }

    void KeyInput::setType(unsigned int type)
    {
        mType = type;
    }

    int KeyInput::getType() const
    {
        return mType;
    }

    void KeyInput::setKey(Key const & key)
    {
        mKey = key;
    }

    Key const & KeyInput::getKey() const
    {
        return mKey;
    }

    bool KeyInput::isShiftPressed() const
    {
        return mShiftPressed;
    }

    void KeyInput::setShiftPressed(bool pressed)
    {
        mShiftPressed = pressed;
    }

    bool KeyInput::isControlPressed() const
    {
        return mControlPressed;
    }

    void KeyInput::setControlPressed(bool pressed)
    {
        mControlPressed = pressed;
    }

    bool KeyInput::isAltPressed() const
    {
        return mAltPressed;
    }

    void KeyInput::setAltPressed(bool pressed)
    {
        mAltPressed = pressed;
    }

    bool KeyInput::isMetaPressed() const
    {
        return mMetaPressed;
    }

    void KeyInput::setMetaPressed(bool pressed)
    {
        mMetaPressed = pressed;
    }

    bool KeyInput::isNumericPad() const
    {
        return mNumericPad;
    }

    void KeyInput::setNumericPad(bool numpad)
    {
        mNumericPad = numpad;
    }
} // namespace fcn
