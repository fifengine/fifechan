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

    /**
     * Drag and drop event type container.
     *
     * Represents drag related events such as entering
     * a widget, hovering, dropping or cancellation.
     */
    class FIFEGUI_API DragEvent : public Event
    {
        public:
            /**
             * Event for drag and drop actions.
             */
            enum class Type : std::uint8_t
            {
                Enter,
                Leave,
                Hover,
                Drop,
                Cancel
            };

            /**
             * Construct a drag event.
             */
            DragEvent(Widget* source, DragPayload const * payload, Type type, int x, int y, int screenX, int screenY);

            /**
             * Get the event type.
             */
            Type getType() const;

            /**
             * Get the payload for this event.
             */
            DragPayload const * getPayload() const;

            /**
             * Get the X coordinate relative to the widget.
             */
            int getX() const;

            /**
             * Get the Y coordinate relative to the widget.
             */
            int getY() const;

            /**
             * Get the X coordinate on the screen.
             */
            int getScreenX() const;

            /**
             * Get the Y coordinate on the screen.
             */
            int getScreenY() const;

        protected:
            /**
             * The event type.
             */
            Type mType;

            /**
             * The associated payload for this event.
             */
            DragPayload const * mPayload;

            /**
             * X coordinate relative to the widget.
             */
            int mX;

            /**
             * Y coordinate relative to the widget.
             */
            int mY;

            /**
             * X coordinate on the screen.
             */
            int mScreenX;

            /**
             * Y coordinate on the screen.
             */
            int mScreenY;

            friend class DragHandler;
    };
} // namespace fcn

#endif // INCLUDE_FIFECHAN_EVENTS_DRAG_EVENT_HPP_
