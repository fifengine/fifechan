// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef FCN_ACTIONEVENT_HPP
#define FCN_ACTIONEVENT_HPP

#include "fifechan/event.hpp"
#include "fifechan/platform.hpp"

#include <string>

namespace fcn
{
    class Widget;

    /**
     * Represents an action event. An action event is an event
     * that can be fired by a widget whenever an action has occured.
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
     * has occured so action listeners of the widget can be informed
     * of the state of the widget.
     *
     * @see Widget::addActionListener, Widget::removeActionListener,
     *      Widget::distributeActionEvent
     */
    class FCN_CORE_DECLSPEC ActionEvent : public Event
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
         * Destructor.
         */
        ~ActionEvent() override;

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

#endif // FCN_ACTIONEVENT_HPP
