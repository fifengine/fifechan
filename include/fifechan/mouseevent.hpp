/***************************************************************************
 *   Copyright (c) 2017 by the fifechan team                               *
 *   https://github.com/fifengine/fifechan                                 *
 *   This file is part of fifechan.                                        *
 *                                                                         *
 *   fifechan is free software; you can redistribute it and/or             *
 *   modify it under the terms of the GNU Lesser General Public            *
 *   License as published by the Free Software Foundation; either          *
 *   version 2.1 of the License, or (at your option) any later version.    *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *   Lesser General Public License for more details.                       *
 *                                                                         *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this library; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA          *
 ***************************************************************************/

/*      _______   __   __   __   ______   __   __   _______   __   __
 *     / _____/\ / /\ / /\ / /\ / ____/\ / /\ / /\ / ___  /\ /  |\/ /\
 *    / /\____\// / // / // / // /\___\// /_// / // /\_/ / // , |/ / /
 *   / / /__   / / // / // / // / /    / ___  / // ___  / // /| ' / /
 *  / /_// /\ / /_// / // / // /_/_   / / // / // /\_/ / // / |  / /
 * /______/ //______/ //_/ //_____/\ /_/ //_/ //_/ //_/ //_/ /|_/ /
 * \______\/ \______\/ \_\/ \_____\/ \_\/ \_\/ \_\/ \_\/ \_\/ \_\/
 *
 * Copyright (c) 2004 - 2008 Olof Naessén and Per Larsson
 *
 *
 * Per Larsson a.k.a finalman
 * Olof Naessén a.k.a jansem/yakslem
 *
 * Visit: http://guichan.sourceforge.net
 *
 * License: (BSD)
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name of Guichan nor the names of its contributors may
 *    be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

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
     *
     * @author Olof Naessén
     */
    class FCN_CORE_DECLSPEC MouseEvent: public InputEvent
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
        MouseEvent(Widget* source,
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
            Middle
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
}

#endif // FCN_MOUSEEVENT_HPP
