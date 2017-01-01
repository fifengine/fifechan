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

#ifndef FCN_MOUSEINPUT_HPP
#define FCN_MOUSEINPUT_HPP

#include "fifechan/platform.hpp"

namespace fcn
{

    /**
     * Internal class that represents mouse input. Generally you won't have to
     * bother using this class unless you implement an Input class for
     * a back end.
     *
     * @author Olof Naessén
     * @author Per Larsson
     */
    class FCN_CORE_DECLSPEC MouseInput
    {
    public:

        /**
         * Constructor.
         */
        MouseInput() { };

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
        MouseInput(unsigned int button,
                   unsigned int type,
                   int x,
                   int y,
                   int timeStamp);

        /**
         * Sets the type of the mouse input.
         *
         * @param type The type of the mouse input. Should be a value from the
         *             mouse event type enum
         * @see getType
         */
        void setType(unsigned int type);

        /**
         * Gets the type of the mouse input.
         *
         * @return The type of the mouse input. A value from the mouse event
         *         type enum.
         * @see setType
         */
        unsigned int getType() const;

        /**
         * Sets the button pressed.
         *
         * @param button The button pressed. Should be one of the values
         *               in the mouse event button enum.
         * @see getButton.
         */
        void setButton(unsigned int button);

        /**
         * Gets the button pressed.
         *
         * @return The button pressed. A value from the mouse event
         *         button enum.
         * @see setButton
         */
        unsigned int getButton() const;

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

        /**
         * Mouse input event types. This enum partially corresponds
         * to the enum with event types in MouseEvent for easy mapping.
         */
        enum
        {
            Moved = 0,
            Pressed,
            Released,
            WheelMovedDown,
            WheelMovedUp
        };

        /**
         * Mouse button types.
         */
		enum {
			Empty = 0,
			Left,
			Right,
			Middle,
			X1,
			X2
		};

    protected:
        /**
         * Holds the type of the mouse input.
         */
        unsigned int mType;

        /**
         * Holds the button of the mouse input.
         */
        unsigned int mButton;

        /** 
         * Holds the timestamp of the mouse input. Used to 
         * check for double clicks.
         */
        int mTimeStamp;

        /** 
         * Holds the x coordinate of the mouse input.
         */
        int mX;

        /** 
         * Holds the y coordinate of the mouse input.
         */
        int mY;
    };
}

#endif // end FCN_MOUSEINPUT_HPP
