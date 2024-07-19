// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef FCN_MOUSEEVENT_HPP
#define FCN_MOUSEEVENT_HPP

#include "fifechan/inputevent.hpp"
#include "fifechan/platform.hpp"

namespace fcn
{
    class Gui;
    class Widget;

    /**
     * Represents a mouse event.
     */
    class FIFEGUI_API MouseEvent : public InputEvent
    {
    public:
        /**
         * Constructor.
         *
         * @param source The widget the event concerns.
         * @param distributor The distributor of the mouse event.
         * @param isShiftPressed True if shift is pressed, false otherwise.
         * @param isControlPressed True if control is pressed, false otherwise.
         * @param isAltPressed True if alt is pressed, false otherwise.
         * @param isMetaPressed True if meta is pressed, false otherwise.
         * @param type The type of the mouse event.
         * @param button The button of the mouse event.
         * @param x The x coordinate of the event relative to the source widget.
         * @param y The y coordinate of the event relative the source widget.
         * @param clickCount The number of clicks generated with the same button.
         *                   It's set to zero if another button is used.
         */
        MouseEvent(
            Widget* source,
            Widget* distributor,
            bool isShiftPressed,
            bool isControlPressed,
            bool isAltPressed,
            bool isMetaPressed,
            unsigned int type,
            unsigned int button,
            int x,
            int y,
            int clickCount);

        /**
         * Gets the button of the mouse event.
         *
         * @return The button of the mouse event.
         */
        unsigned int getButton() const;

        /**
         * Gets the x coordinate of the mouse event.
         * The coordinate relative to widget the mouse listener
         * receiving the events have registered to.
         *
         * @return The x coordinate of the mouse event.
         * @see Widget::addMouseListener, Widget::removeMouseListener
         */
        int getX() const;

        /**
         * Gets the y coordinate of the mouse event.
         * The coordinate relative to widget the mouse listener
         * receiving the events have registered to.
         *
         * @return The y coordinate of the mouse event.
         * @see Widget::addMouseListener, Widget::removeMouseListener
         */
        int getY() const;

        /**
         * Gets the number of clicks generated with the same button.
         * It's set to zero if another button is used.
         *
         * @return The number of clicks generated with the same button.
         */
        int getClickCount() const;

        /**
         * Gets the type of the event.
         *
         * @return The type of the event.
         */
        unsigned int getType() const;

        /**
         * Mouse event types.
         */
        enum
        {
            Moved = 0,
            Pressed,
            Released,
            WheelMovedDown,
            WheelMovedUp,
            WheelMovedRight,
            WheelMovedLeft,
            Clicked,
            Entered,
            Exited,
            Dragged

        };

        /**
         * Mouse button types.
         */
        enum
        {
            Empty = 0,
            Left,
            Right,
            Middle,
            X1,
            X2
        };

    protected:
        /**
         * Holds the type of the mouse event.
         */
        unsigned int mType;

        /**
         * Holds the button of the mouse event.
         */
        unsigned int mButton;

        /**
         * Holds the x-coordinate of the mouse event.
         */
        int mX;

        /**
         * Holds the y-coordinate of the mouse event.
         */
        int mY;

        /**
         * The number of clicks generated with the same button.
         * It's set to zero if another button is used.
         */
        int mClickCount;

        /**
         * Gui is a friend of this class in order to be able to manipulate
         * the protected member variables of this class and at the same time
         * keep the MouseEvent class as const as possible. Gui needs to
         * update the x och y coordinates for the coordinates to be relative
         * to widget the mouse listener receiving the events have registered
         * to.
         */
        friend class Gui;
    };
} // namespace fcn

#endif // FCN_MOUSEEVENT_HPP
