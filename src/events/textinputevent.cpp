// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Fifengine contributors

// Corresponding header include
#include "fifechan/events/textinputevent.hpp"

namespace fcn
{
    TextInputEvent::TextInputEvent(
        Widget* source,
        Widget* distributor,
        bool isShiftPressed,
        bool isControlPressed,
        bool isAltPressed,
        bool isMetaPressed,
        std::string text,
        Type type) :
        InputEvent(source, distributor, isShiftPressed, isControlPressed, isAltPressed, isMetaPressed),
        mText(std::move(text)),
        mType(type)
    {
    }

    std::string const & TextInputEvent::getText() const
    {
        return mText;
    }

    TextInputEvent::Type TextInputEvent::getType() const
    {
        return mType;
    }
} // namespace fcn
