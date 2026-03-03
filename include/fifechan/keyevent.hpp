// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_KEYEVENT_HPP_
#define INCLUDE_FIFECHAN_KEYEVENT_HPP_

#include <cstdint>

#include "fifechan/inputevent.hpp"
#include "fifechan/key.hpp"
#include "fifechan/platform.hpp"

namespace fcn
{
    class Widget;

    /**
     * Represents a key event.
     */
    class FIFEGUI_API KeyEvent : public InputEvent
    {
    public:
        /**
         * Key event types.
         */
        enum class Type : std::uint8_t
        {
            Pressed = 0,
            Released
        };

        /**
         * Constructor.
         *
         * @param source The widget the event concerns..
         * @param distributor The distributor of the event.
         * @param isShiftPressed True if shift is pressed, false otherwise.
         * @param isControlPressed True if control is pressed, false otherwise.
         * @param isAltPressed True if alt is pressed, false otherwise.
         * @param isMetaPressed True if meta is pressed, false otherwise.
         * @param type The type of the event. A value from KeyEventType.
         * @param isNumericPad True if the event occurred on the numeric pad,
         *                     false otherwise.
         * @param key The key of the event.
         */
        KeyEvent(
            Widget* source,
            Widget* distributor,
            bool isShiftPressed,
            bool isControlPressed,
            bool isAltPressed,
            bool isMetaPressed,
            unsigned int type,
            bool isNumericPad,
            Key const & key);

        ~KeyEvent() override;

        /**
         * Gets the type of the event.
         *
         * @return The type of the event.
         */
        unsigned int getType() const;

        /**
         * Checks if the key event occurred on the numeric pad.
         *
         * @return True if key event occurred on the numeric pad,
         *         false otherwise.
         *
         */
        bool isNumericPad() const;

        /**
         * Gets the key of the event.
         *
         * @return The key of the event.
         */
        Key const & getKey() const;

    protected:
        /**
         * Holds the type of the key event.
         */
        unsigned int mType;

        /**
         * True if the numeric pad was used, false otherwise.
         */
        bool mIsNumericPad;

        /**
         * Holds the key of the key event.
         */
        Key mKey;
    };
} // namespace fcn

#endif // INCLUDE_FIFECHAN_KEYEVENT_HPP_
