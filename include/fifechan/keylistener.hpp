// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef FCN_KEYLISTENER_HPP
#define FCN_KEYLISTENER_HPP

#include "fifechan/keyevent.hpp"
#include "fifechan/platform.hpp"

namespace fcn
{
    class Key;

    /**
     * Interface for listening for key events from widgets.
     *
     * @see Widget::addKeyListener, Widget::removeKeyListener
     */
    class FCN_CORE_DECLSPEC KeyListener
    {
    public:
        /**
         * Destructor.
         */
        virtual ~KeyListener() = default;

        /**
         * Called if a key is pressed when the widget has keyboard focus.
         * If a key is held down the widget will generate multiple key
         * presses.
         *
         * @param keyEvent Discribes the event.
         */
        virtual void keyPressed(KeyEvent& keyEvent) { }

        /**
         * Called if a key is released when the widget has keyboard focus.
         *
         * @param keyEvent Discribes the event.
         */
        virtual void keyReleased(KeyEvent& keyEvent) { }

    protected:
        /**
         * Constructor.
         *
         * You should not be able to make an instance of KeyListener,
         * therefore its constructor is protected.
         */
        KeyListener() { }
    };
} // namespace fcn

#endif // end FCN_KEYLISTENER_HPP
