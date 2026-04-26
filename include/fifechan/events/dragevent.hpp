// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_EVENTS_DRAG_EVENT_HPP_
#define INCLUDE_FIFECHAN_EVENTS_DRAG_EVENT_HPP_

#include <cstdint>

#include "fifechan/events/event.hpp"

namespace fcn
{
    class Widget;
    class DragPayload;

    class FIFEGUI_API DragEvent : public Event
    {
    public:
        enum class Type : std::uint8_t
        {
            Enter,
            Leave,
            Hover,
            Drop,
            Cancel
        };

        DragEvent(Widget* source, DragPayload const * payload, Type type, int x, int y, int screenX, int screenY);

        Type getType() const;
        DragPayload const * getPayload() const;
        int getX() const;
        int getY() const;
        int getScreenX() const;
        int getScreenY() const;

    protected:
        Type mType;
        DragPayload const * mPayload;
        int mX, mY;
        int mScreenX, mScreenY;

        friend class DragHandler;
    };
} // namespace fcn

#endif // INCLUDE_FIFECHAN_EVENTS_DRAG_EVENT_HPP_
