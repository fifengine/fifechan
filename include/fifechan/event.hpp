// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef FCN_EVENT_HPP
#define FCN_EVENT_HPP

#include "fifechan/platform.hpp"

namespace fcn
{
    class Widget;

    /**
     * Base class for all events. All events in Guichan should inherit from this class.
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
         * Destructor.
         */
        virtual ~Event();

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
        Widget* mSource;
    };
} // namespace fcn

#endif // end FCN_EVENT_HPP
