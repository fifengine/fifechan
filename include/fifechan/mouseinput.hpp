// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_MOUSEINPUT_HPP_
#define INCLUDE_FIFECHAN_MOUSEINPUT_HPP_

#include <cstdint>

#include "fifechan/platform.hpp"

namespace fcn
{

    /**
     * Internal class representing raw mouse input data.
     *
     * Generally you won't have to bother using this class unless you implement
     * an Input class fora backend.
     *
     * @ingroup input
     */
    class FIFEGUI_API MouseInput
    {
    public:
        MouseInput() = default;

        enum class Type : std::uint8_t
        {
            Moved = 0,
            Pressed,
            Released,
            WheelMovedDown,
            WheelMovedUp,
            WheelMovedRight,
            WheelMovedLeft
        };

        enum class Button : std::uint8_t
        {
            Empty = 0,
            Left,
            Right,
            Middle,
            X1,
            X2
        };

        /**
         * Constructor.
         *
         * @param button The button pressed.
         * @param type The type of mouse input.
         * @param x The mouse x coordinate.
         * @param y The mouse y coordinate.
         * @param timeStamp The timestamp of the mouse input. Used to
         *                  check for double clicks.
         */
        MouseInput(Button button, Type type, int x, int y, int timeStamp);

        /**
         * Sets the type of the mouse input.
         *
         * @param type The type of the mouse input.
         * @see getType
         */
        void setType(Type type);

        /**
         * Gets the type of the mouse input.
         *
         * @return The type of the mouse input.
         * @see setType
         */
        Type getType() const;

        /**
         * Sets the button pressed.
         *
         * @param button The button pressed.
         * @see getButton.
         */
        void setButton(Button button);

        /**
         * Gets the button pressed.
         *
         * @return The button pressed.
         * @see setButton
         */
        Button getButton() const;

        /**
         * Sets the timestamp for the mouse input.
         * Used to check for double clicks.
         *
         * @param timeStamp The timestamp of the mouse input.
         * @see getTimeStamp
         */
        void setTimeStamp(int timeStamp);

        /**
         * Gets the time stamp of the input.
         * Used to check for double clicks.
         *
         * @return The time stamp of the mouse input.
         * @see setTimeStamp
         */
        int getTimeStamp() const;

        /**
         * Sets the x coordinate of the mouse input.
         *
         * @param x The x coordinate of the mouse input.
         * @see getX
         */
        void setX(int x);

        /**
         * Gets the x coordinate of the mouse input.
         *
         * @return The x coordinate of the mouse input.
         * @see setX
         */
        int getX() const;

        /**
         * Sets the y coordinate of the mouse input.
         *
         * @param y The y coordinate of the mouse input.
         * @see getY
         */
        void setY(int y);

        /**
         * Gets the y coordinate of the mouse input.
         *
         * @return The y coordinate of the mouse input.
         * @see setY
         */
        int getY() const;

    protected:
        /**
         * Holds the type of the mouse input.
         */
        Type mType{Type::Moved};

        /**
         * Holds the button of the mouse input.
         */
        Button mButton{Button::Empty};

        /**
         * Holds the timestamp of the mouse input. Used to
         * check for double clicks.
         */
        int mTimeStamp{0};

        /**
         * Holds the x coordinate of the mouse input.
         */
        int mX{0};

        /**
         * Holds the y coordinate of the mouse input.
         */
        int mY{0};
    };
} // namespace fcn

#endif // INCLUDE_FIFECHAN_MOUSEINPUT_HPP_
