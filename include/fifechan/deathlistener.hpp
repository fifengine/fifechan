// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_DEATHLISTENER_HPP_
#define INCLUDE_FIFECHAN_DEATHLISTENER_HPP_

#include <string>

#include "fifechan/event.hpp"
#include "fifechan/platform.hpp"

namespace fcn
{
    /**
     * Interface for listening for death events from widgets.
     *
     * @see Widget::addDeathListener, Widget::removeDeathListener
     */
    class FIFEGUI_API DeathListener
    {
    public:
        virtual ~DeathListener() = default;

        DeathListener(DeathListener const &)            = delete;
        DeathListener& operator=(DeathListener const &) = delete;
        DeathListener(DeathListener&&)                  = delete;
        DeathListener& operator=(DeathListener&&)       = delete;

        /**
         * Called when a widget dies. It is used to be able to receive
         * a notification when a death of a widget occurs.
         *
         * @param event The event of the death.
         */
        virtual void death(Event const & event) = 0;

    protected:
        /**
         * Constructor.
         *
         * Instances should not be created directly,
         * therefore the constructor is declared protected.
         */
        DeathListener() = default;
    };
} // namespace fcn

#endif // INCLUDE_FIFECHAN_DEATHLISTENER_HPP_
