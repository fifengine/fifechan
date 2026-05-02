// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2013 - 2026 Fifeengine contributors

// Corresponding header include
#include "fifechan/events/dragevent.hpp"

// Standard library includes
#include <cassert>

// Platform config include
#include "fifechan/platform.hpp"

namespace fcn
{
    DragEvent::DragEvent(
        Widget* source, DragPayload const * payload, Type type, int x, int y, int screenX, int screenY) :
        Event(source), mType(type), mPayload(payload), mX(x), mY(y), mScreenX(screenX), mScreenY(screenY)
    {
        assert(
            "Type must be valid" && (type == Type::Enter || type == Type::Leave || type == Type::Hover ||
                                     type == Type::Drop || type == Type::Cancel));
        assert("Source widget must not be null" && source != nullptr);
    }

    DragEvent::Type DragEvent::getType() const
    {
        return mType;
    }
    DragPayload const * DragEvent::getPayload() const
    {
        return mPayload;
    }
    int DragEvent::getX() const
    {
        return mX;
    }
    int DragEvent::getY() const
    {
        return mY;
    }
    int DragEvent::getScreenX() const
    {
        return mScreenX;
    }
    int DragEvent::getScreenY() const
    {
        return mScreenY;
    }

} // namespace fcn
