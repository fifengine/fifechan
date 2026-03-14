// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_ACTIONEVENT_HPP_
#define INCLUDE_FIFECHAN_ACTIONEVENT_HPP_

#include <string>

#include "fifechan/event.hpp"
#include "fifechan/platform.hpp"

namespace fcn
{
    class Widget;

    /**
     * Represents an action trigger (e.g., button click).
     *
     * An action event is an event that can be fired by a widget
     * whenever an action has occurred.
     * What exactly an action is is up to the widget that fires
     * the action event. An example is a Button which fires an action
     * event as soon as the Button is clicked, another example is
     * TextField which fires an action event as soon as the enter
     * key is pressed.
     *
     * Any object can listen for actions from widgets by implementing
     * the ActionListener interface.
     *
     * If you have implement a widget of your own it's a good idea to
     * let the widget fire action events whenever you feel an action
     * has occurred so action listeners of the widget can be informed
     * of the state of the widget.
     *
     * @see Widget::addActionListener, Widget::removeActionListener,
     *      Widget::distributeActionEvent
     *
     * @ingroup events
     */
    class FIFEGUI_API ActionEvent : public Event
    {
    public:
        /**
         * Constructor.
         *
         * @param source The source widget of the event.
         * @param id An identifier of the event.
         */
        ActionEvent(Widget* source, std::string id);

        /**
         * Gets the identifier of the event. An identifier can
         * be used to distinguish from two actions from the same
         * widget or to let many widgets fire the same widgets
         * that should be treated equally.
         *
         * @return The identifier of the event.
         */
        std::string const & getId() const;

    protected:
        /**
         * Holds the identifier of the event.
         */
        std::string mId;
    };
} // namespace fcn

#endif // INCLUDE_FIFECHAN_ACTIONEVENT_HPP_
