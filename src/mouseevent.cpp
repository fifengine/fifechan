// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

#include "fifechan/mouseevent.hpp"

#include "fifechan/inputevent.hpp"

namespace fcn
{
    MouseEvent::MouseEvent(
        Widget* source,
        Widget* distributor,
        bool isShiftPressed,
        bool isControlPressed,
        bool isAltPressed,
        bool isMetaPressed,
        MouseEvent::Type type,
        MouseEvent::Button button,
        int x,
        int y,
        int clickCount) :
        InputEvent(source, distributor, isShiftPressed, isControlPressed, isAltPressed, isMetaPressed),
        mType(type),
        mButton(button),
        mX(x),
        mY(y),
        mClickCount(clickCount)
    {
    }

    MouseEvent::Button MouseEvent::getButton() const
    {
        return mButton;
    }

    int MouseEvent::getX() const
    {
        return mX;
    }

    int MouseEvent::getY() const
    {
        return mY;
    }

    int MouseEvent::getClickCount() const
    {
        return mClickCount;
    }

    MouseEvent::Type MouseEvent::getType() const
    {
        return mType;
    }
} // namespace fcn
