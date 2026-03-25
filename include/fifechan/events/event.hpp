// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_EVENT_HPP_
#define INCLUDE_FIFECHAN_EVENT_HPP_

#include "fifechan/platform.hpp"

namespace fcn
{
    class Widget;

    /**
     * Base class for all GUI event objects.
     *
     * An event is a signal that something has happened in the GUI.
     * Events are used to notify widgets of user actions.
     * The event class is the base class for all events in the GUI.
     *
     * @ingroup events
     */
    class FIFEGUI_API Event
    {
    public:
        /**
         * Constructor.
         *
         * @param source The source widget of the event.
         */
        explicit Event(Widget* source);

        /**
         * Gets the source widget of the event. The function
         * is used to tell which widget fired an event.
         *
         * @return The source widget of the event.
         */
        Widget* getSource() const;

    protected:
        /**
         * Holds the source widget of the event.
         */
        Widget* mSource{nullptr};
    };
} // namespace fcn

#endif // INCLUDE_FIFECHAN_EVENT_HPP_
