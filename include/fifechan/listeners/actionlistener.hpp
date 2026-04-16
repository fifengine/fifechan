// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_ACTIONLISTENER_HPP_
#define INCLUDE_FIFECHAN_ACTIONLISTENER_HPP_

// Standard library includes
#include <string>

// Platform config include
#include "fifechan/platform.hpp"

// Project headers (subdirs before local)
#include "fifechan/events/actionevent.hpp"

namespace fcn
{
    /**
     * Interface for listening to action events from widgets.
     *
     * @see Widget::addActionListener, Widget::removeActionListener,
     *      ActionEvent
     *
     * @ingroup listeners
     */
    class FIFEGUI_API ActionListener
    {
    public:
        virtual ~ActionListener() = default;

        ActionListener(ActionListener const &)            = delete;
        ActionListener& operator=(ActionListener const &) = delete;
        ActionListener(ActionListener&&)                  = delete;
        ActionListener& operator=(ActionListener&&)       = delete;

        /**
         * Handles an action event emitted by a widget.
         *
         * Invoked when a widget triggers an action, providing details about
         * the event via the ActionEvent object.
         *
         * @param actionEvent The action event data.
         */
        virtual void action(ActionEvent const & actionEvent) = 0;

    protected:
        /**
         * Protected default constructor.
         *
         * Instances should not be created directly,
         * therefore the constructor is declared protected.
         */
        ActionListener() = default;
    };
} // namespace fcn

#endif // INCLUDE_FIFECHAN_ACTIONLISTENER_HPP_
