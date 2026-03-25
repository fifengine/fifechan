// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

#include "fifechan/events/inputevent.hpp"

#include "fifechan/events/event.hpp"

namespace fcn
{
    InputEvent::InputEvent(
        Widget* source,
        Widget* distributor,
        bool isShiftPressed,
        bool isControlPressed,
        bool isAltPressed,
        bool isMetaPressed) :
        Event(source),
        mShiftPressed(isShiftPressed),
        mControlPressed(isControlPressed),
        mAltPressed(isAltPressed),
        mMetaPressed(isMetaPressed),
        mIsConsumed(false),
        mDistributor(distributor)
    {
    }

    bool InputEvent::isShiftPressed() const
    {
        return mShiftPressed;
    }

    bool InputEvent::isControlPressed() const
    {
        return mControlPressed;
    }

    bool InputEvent::isAltPressed() const
    {
        return mAltPressed;
    }

    bool InputEvent::isMetaPressed() const
    {
        return mMetaPressed;
    }

    void InputEvent::consume()
    {
        mIsConsumed = true;
    }

    bool InputEvent::isConsumed() const
    {
        return mIsConsumed;
    }

    Widget* InputEvent::getDistributor() const
    {
        return mDistributor;
    }
} // namespace fcn
