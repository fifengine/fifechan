// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

#include "fifechan/events/keyevent.hpp"

#include "fifechan/events/inputevent.hpp"
#include "fifechan/key.hpp"

namespace fcn
{
    KeyEvent::KeyEvent(
        Widget* source,
        Widget* distributor,
        bool isShiftPressed,
        bool isControlPressed,
        bool isAltPressed,
        bool isMetaPressed,
        Type type,
        bool isNumericPad,
        Key const & key) :
        InputEvent(source, distributor, isShiftPressed, isControlPressed, isAltPressed, isMetaPressed),
        mType(type),
        mIsNumericPad(isNumericPad),
        mKey(key)
    {
    }

    KeyEvent::Type KeyEvent::getType() const
    {
        return mType;
    }

    bool KeyEvent::isNumericPad() const
    {
        return mIsNumericPad;
    }

    Key const & KeyEvent::getKey() const
    {
        return mKey;
    }
} // namespace fcn
