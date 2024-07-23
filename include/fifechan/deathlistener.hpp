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

        /**
         * Called when a widget dies. It is used to be able to recieve
         * a notification when a death of a widget occurs.
         *
         * @param event The event of the death.
         */
        virtual void death(Event const & event) = 0;

    protected:
        /**
         * Constructor.
         *
         * You should not be able to make an instance of DeathListener,
         * therefore its constructor is protected.
         */
        DeathListener() { }
    };
} // namespace fcn

#endif // INCLUDE_FIFECHAN_DEATHLISTENER_HPP_
