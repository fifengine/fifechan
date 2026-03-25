// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_KEYLISTENER_HPP_
#define INCLUDE_FIFECHAN_KEYLISTENER_HPP_

#include "fifechan/events/keyevent.hpp"
#include "fifechan/platform.hpp"

namespace fcn
{
    class Key;

    /**
     * Interface for listening to keyboard events.
     *
     * @see Widget::addKeyListener, Widget::removeKeyListener
     *
     * @ingroup listeners
     */
    class FIFEGUI_API KeyListener
    {
    public:
        virtual ~KeyListener() = default;

        /** Copy constructor. */
        KeyListener(KeyListener const &) = default;
        /** Copy assignment operator. */
        KeyListener& operator=(KeyListener const &) = default;
        /** Move constructor. */
        KeyListener(KeyListener&&) = default;
        /** Move assignment operator. */
        KeyListener& operator=(KeyListener&&) = default;

        /**
         * Called if a key is pressed when the widget has keyboard focus.
         * If a key is held down the widget will generate multiple key
         * presses.
         *
         * @param keyEvent Describes the event.
         */
        virtual void keyPressed(KeyEvent& keyEvent) { }

        /**
         * Called if a key is released when the widget has keyboard focus.
         *
         * @param keyEvent Describes the event.
         */
        virtual void keyReleased(KeyEvent& keyEvent) { }

    protected:
        /**
         * Constructor.
         *
         * Instances should not be created directly,
         * therefore the constructor is declared protected.
         */
        KeyListener() = default;
    };
} // namespace fcn

#endif // INCLUDE_FIFECHAN_KEYLISTENER_HPP_
