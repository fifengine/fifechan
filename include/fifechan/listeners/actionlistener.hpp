// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_ACTIONLISTENER_HPP_
#define INCLUDE_FIFECHAN_ACTIONLISTENER_HPP_

#include <string>

#include "fifechan/actionevent.hpp"
#include "fifechan/platform.hpp"

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
         * Called when an action is received from a widget.
         * It is used to receive notifications about actions that have occurred.
         * The actionEvent contains information about the action.
         *
         * @param actionEvent The event of the action.
         */
        virtual void action(ActionEvent const & actionEvent) = 0;

    protected:
        /**
         * Constructor.
         *
         * Instances should not be created directly,
         * therefore the constructor is declared protected.
         */
        ActionListener() = default;
    };
} // namespace fcn

#endif // INCLUDE_FIFECHAN_ACTIONLISTENER_HPP_
