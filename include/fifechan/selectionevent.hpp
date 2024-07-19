// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef FCN_SELECTIONEVENT_HPP
#define FCN_SELECTIONEVENT_HPP

#include "fifechan/event.hpp"
#include "fifechan/platform.hpp"

namespace fcn
{
    class Widget;

    /**
     * Represents a selection event.
     */
    class FCN_CORE_DECLSPEC SelectionEvent : public Event
    {
    public:
        /**
         * Constructor.
         *
         * @param source source The widget of the selection event.
         */
        explicit SelectionEvent(Widget* source);

        /**
         * Destructor.
         */
        ~SelectionEvent() override;
    };
} // namespace fcn

#endif // end FCN_SELECTIONEVENT_HPP
