// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_KEYINPUT_HPP_
#define INCLUDE_FIFECHAN_KEYINPUT_HPP_

#include <cstdint>

#include "fifechan/key.hpp"
#include "fifechan/platform.hpp"

namespace fcn
{
    /**
     * Internal class representing raw keyboard input data.
     *
     * This class is used to represent key input events.
     * It holds information about the key that was pressed or released,
     * the type of the key input event and if any modifier keys were pressed
     * at the same time as the key.
     *
     * The class is relevant if you want to implement an Input class for a backend.
     *
     * @ingroup input
     */
    class FIFEGUI_API KeyInput
    {
    public:
        /**
         * Key input event types.
         * This enum corresponds to the enum with event types
         * on KeyEvent for easy mapping.
         */
        enum class Type : std::uint8_t
        {
            Pressed = 0,
            Released
        };

        /**
         * Constructor.
         */
        KeyInput() = default;

        /**
         * Constructor.
         *
         * @param key The key of the key input.
         * @param type The type of key input.
         */
        KeyInput(Key const & key, Type type);

        /**
         * Sets the type of the key input.
         *
         * @param type The type of key input.
         * @see getType
         */
        void setType(Type type);

        /**
         * Gets the type of the key input.
         *
         * @return the input type.
         * @see setType
         */
        Type getType() const;

        /**
         * Sets the key of the key input.
         *
         * @param key The key of the key input.
         * @see getKey
         */
        void setKey(Key const & key);

        /**
         * Gets the key of the key input.
         *
         * @return The key of the key input.
         * @see setKey
         */
        Key const & getKey() const;

        /**
         * Checks if shift is pressed.
         *
         * @return True if shift was pressed at the same
         *         time as the key, false otherwise.
         * @see setShiftPressed
         */
        bool isShiftPressed() const;

        /**
         * Sets shift to be pressed at the same time as the key,
         * or not.
         *
         * @param pressed True if shift is pressed, false otherwise.
         * @see isShiftPressed
         */
        void setShiftPressed(bool pressed);

        /**
         * Checks if control is pressed.
         *
         * @return True if control was pressed at the same
         *         time as the key, false otherwise.
         * @see setControlPressed
         */
        bool isControlPressed() const;

        /**
         * Sets control to be pressed at the same time as the key,
         * or not.
         *
         * @param pressed True if control is pressed, false otherwise.
         * @see isControlPressed
         */
        void setControlPressed(bool pressed);

        /**
         * Checks if alt is pressed.
         *
         * @return True if alt was pressed at the same
         *         time as the key, false otherwise.
         * @see setAltPressed
         */
        bool isAltPressed() const;

        /**
         * Sets the alt to be pressed at the same time as the key,
         * or not.
         *
         * @param pressed True if alt is pressed at the same
         *                time as the key, , false otherwise.
         * @see isAltPressed
         */
        void setAltPressed(bool pressed);

        /**
         * Checks if meta is pressed.
         *
         * @return True if meta was pressed at the same
         *         time as the key, false otherwise.
         * @see setMetaPressed
         */
        bool isMetaPressed() const;

        /**
         * Sets meta to be pressed at the same time as the key,
         * or not.
         *
         * @param pressed True if meta is pressed at the same
         *                time as the key, false otherwise.
         * @see isMetaPressed
         */
        void setMetaPressed(bool pressed);

        /**
         * Checks if the key was pressed at the numeric pad.
         *
         * @return True if key pressed at the numeric pad,
         *         false otherwise.
         * @see setNumericPad
         */
        bool isNumericPad() const;

        /**
         * Sets the key to be pressed at the numeric pad.
         *
         * @param numpad True if the key was pressed at the numeric
         *               pad, false otherwise.
         * @see isNumericPad
         */
        void setNumericPad(bool numpad);

    protected:
        /**
         * Holds the key of the key input.
         */
        Key mKey{};

        /**
         * Holds the type of the key input.
         */
        Type mType{Type::Released};

        /**
         * True if shift was pressed at the same time as the key,
         * false otherwise.
         */
        bool mShiftPressed{false};

        /**
         * True if control was pressed at the same time as the key,
         * false otherwise.
         */
        bool mControlPressed{false};

        /**
         * True if alt was pressed at the same time as the key,
         * false otherwise.
         */
        bool mAltPressed{false};

        /**
         * True if meta was pressed at the same time as the key,
         * false otherwise.
         */
        bool mMetaPressed{false};

        /**
         * True if the numeric pad was used when the key was pressed,
         * false otherwise.
         */
        bool mNumericPad{false};
    };
} // namespace fcn

#endif // INCLUDE_FIFECHAN_KEYINPUT_HPP_
