// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_ACTIONLISTENER_HPP_
#define INCLUDE_FIFECHAN_ACTIONLISTENER_HPP_

#include <string>

#include "fifechan/actionevent.hpp"
#include "fifechan/platform.hpp"

namespace fcn
{
    /**
     * Interface for listening for action events from widgets.
     *
     * @see Widget::addActionListener, Widget::removeActionListener,
     *      ActionEvent
     */
    class FIFEGUI_API ActionListener
    {
    public:
        virtual ~ActionListener() = default;

        /**
         * Called when an action is recieved from a widget. It is used
         * to be able to recieve a notification that an action has
         * occured.
         *
         * @param actionEvent The event of the action.
         */
        virtual void action(ActionEvent const & actionEvent) = 0;

    protected:
        /**
         * Constructor.
         *
         * You should not be able to make an instance of ActionListener,
         * therefore its constructor is protected.
         */
        ActionListener() { }
    };
} // namespace fcn

#endif // INCLUDE_FIFECHAN_ACTIONLISTENER_HPP_
