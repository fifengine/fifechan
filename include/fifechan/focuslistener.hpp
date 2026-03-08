// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_FOCUSLISTENER_HPP_
#define INCLUDE_FIFECHAN_FOCUSLISTENER_HPP_

#include <string>

#include "fifechan/event.hpp"
#include "fifechan/platform.hpp"

namespace fcn
{
    /**
     * Interface for listening for focus events from widgets.
     *
     * @see Widget::addFocusListener, Widget::removeFocusListener
     */
    class FIFEGUI_API FocusListener
    {
    public:
        virtual ~FocusListener() = default;

        /**
         * Called when a widget gains focus.
         *
         * @param event Describes the event.
         */
        virtual void focusGained(Event const & event) { }

        /**
         * Called when a widget loses focus.
         *
         * @param event Describes the event.
         */
        virtual void focusLost(Event const & event) { }

    protected:
        /**
         * Constructor.
         *
         * You should not be able to make an instance of FocusListener,
         * therefore its constructor is protected.
         */
        FocusListener() = default;
    };
} // namespace fcn

#endif // INCLUDE_FIFECHAN_FOCUSLISTENER_HPP_
