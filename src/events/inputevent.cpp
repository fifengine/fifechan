// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

// Corresponding header include
#include "fifechan/events/inputevent.hpp"

// Standard library includes
#include <cassert>

// Platform config include
#include "fifechan/platform.hpp"

// Project headers (subdirs before local)
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
        Event(source), mIsConsumed(false), mDistributor(distributor)
    {
        (void)source;
        (void)distributor;
        if (isShiftPressed)
            mModMask |= ModShift;
        if (isControlPressed)
            mModMask |= ModControl;
        if (isAltPressed)
            mModMask |= ModAlt;
        if (isMetaPressed)
            mModMask |= ModMeta;
    }

    bool InputEvent::isShiftPressed() const
    {
        return (mModMask & ModShift) != 0;
    }

    bool InputEvent::isControlPressed() const
    {
        return (mModMask & ModControl) != 0;
    }

    bool InputEvent::isAltPressed() const
    {
        return (mModMask & ModAlt) != 0;
    }

    bool InputEvent::isMetaPressed() const
    {
        return (mModMask & ModMeta) != 0;
    }

    uint16_t InputEvent::getModMask() const
    {
        return mModMask;
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
