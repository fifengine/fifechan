// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_SELECTIONEVENT_HPP_
#define INCLUDE_FIFECHAN_SELECTIONEVENT_HPP_

#include "fifechan/event.hpp"
#include "fifechan/platform.hpp"

namespace fcn
{
    class Widget;

    /**
     * Represents a selection event.
     */
    class FIFEGUI_API SelectionEvent : public Event
    {
    public:
        /**
         * Constructor.
         *
         * @param source source The widget of the selection event.
         */
        explicit SelectionEvent(Widget* source);
    };
} // namespace fcn

#endif // INCLUDE_FIFECHAN_SELECTIONEVENT_HPP_
