// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_FOCUSLISTENER_HPP_
#define INCLUDE_FIFECHAN_FOCUSLISTENER_HPP_

#include <string>

#include "fifechan/events/event.hpp"
#include "fifechan/platform.hpp"

namespace fcn
{
    /**
     * Interface for listening to focus gain/loss events.
     *
     * @see Widget::addFocusListener, Widget::removeFocusListener
     *
     * @ingroup listeners
     */
    class FIFEGUI_API FocusListener
    {
    public:
        virtual ~FocusListener() = default;

        FocusListener(FocusListener const &)            = delete;
        FocusListener& operator=(FocusListener const &) = delete;
        FocusListener(FocusListener&&)                  = delete;
        FocusListener& operator=(FocusListener&&)       = delete;

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
         * Instances should not be created directly,
         * therefore the constructor is declared protected.
         */
        FocusListener() = default;
    };
} // namespace fcn

#endif // INCLUDE_FIFECHAN_FOCUSLISTENER_HPP_
