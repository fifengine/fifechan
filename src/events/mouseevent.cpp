// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

// Corresponding header include
#include "fifechan/events/mouseevent.hpp"

// Standard library includes
#include <cassert>

// Platform config include
#include "fifechan/platform.hpp"

// Project headers (subdirs before local)
#include "fifechan/events/inputevent.hpp"

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
        assert(
            "Type must be valid" &&
            (type == MouseEvent::Type::Pressed || type == MouseEvent::Type::Released ||
             type == MouseEvent::Type::Moved || type == MouseEvent::Type::WheelMovedDown ||
             type == MouseEvent::Type::WheelMovedUp || type == MouseEvent::Type::WheelMovedRight ||
             type == MouseEvent::Type::WheelMovedLeft || type == MouseEvent::Type::Clicked ||
             type == MouseEvent::Type::Entered || type == MouseEvent::Type::Exited ||
             type == MouseEvent::Type::Dragged));
        assert(
            "Button must be valid" && (button == MouseEvent::Button::Empty || button == MouseEvent::Button::Left ||
                                       button == MouseEvent::Button::Right || button == MouseEvent::Button::Middle ||
                                       button == MouseEvent::Button::X1 || button == MouseEvent::Button::X2));
        assert("Click count must be non-negative" && clickCount >= 0);
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
