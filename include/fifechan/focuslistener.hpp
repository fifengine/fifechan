// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef FCN_FOCUSLISTENER_HPP
#define FCN_FOCUSLISTENER_HPP

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
    class FCN_CORE_DECLSPEC FocusListener
    {
    public:
        /**
         * Destructor.
         */
        virtual ~FocusListener() = default;

        /**
         * Called when a widget gains focus.
         *
         * @param event Discribes the event.
         */
        virtual void focusGained(Event const & event){};

        /**
         * Called when a widget loses focus.
         *
         * @param event Discribes the event.
         */
        virtual void focusLost(Event const & event){};

    protected:
        /**
         * Constructor.
         *
         * You should not be able to make an instance of FocusListener,
         * therefore its constructor is protected.
         */
        FocusListener() { }
    };
} // namespace fcn

#endif // end FCN_FOCUSLISTENER_HPP
